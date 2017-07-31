/* Timelapse Rail Controller 
 *
 * Stepper pins: 8, 9, 10, 11
 *
 * Bump Switch pin: A0
 *
 * Camera Trigger pin: 3
 *
 * Formula for calculating total delay time:
 * (stabilisation) + (step delay * 8) + (shoot delay)
 *     (1000)      + (step_delay * 8) +     (200)
 *
 * Command Inputs:
 * 
 *    <             | Step Backward
 *    >             | Step Forward
 *    a             | Move to Start (requires bump_stop)
 *    p             | Sets paused/unpaused
 *    r             | Toggle rotation direction
 *    s             | Start/Stop (stop resets frame and step counts) (requires bump stop)
 *    m<step size>  | Sets movement (step) size to the number given
 *    d<delay time> | Sets delay between moves/shots
 *    r<(1 or 0)>   | Sets rotation direction specifically. 1 = clockwise
 *
 */

#include "RailStepper.hpp"

#define trigger 3
#define bump_stop A0
#define stabilisation 1000

#define motor_pin_1 8
#define motor_pin_2 9
#define motor_pin_3 10
#define motor_pin_4 11

RailStepper railStepper(bump_stop, trigger, 
                        motor_pin_1, motor_pin_2, 
                        motor_pin_3, motor_pin_4);

int step_delay = 2;
int step_move = 5;
int step_count = 0;
int frame = 0;

boolean isPaused = true;
boolean isClockwise = false;
boolean isTiming = false;
boolean isReset = false;
boolean isStart = false;

unsigned long delay_time = 1000;
unsigned long start;
unsigned long timeout = millis();

String in = "";

void setup() {
    Serial.begin(9600);
    railStepper.printMove();
    railStepper.printDelay();
}

void loop() {
    railStepper.checkSerial();
    if (!isPaused) {
        railStepper.move(step_move);
        wait(stabilisation);
        railStepper.shoot();
        wait(delay_time);
    } else {
        wait(1000);
    }
    if (millis() >= timeout + 1000) {
        Serial.println('w');
        timeout = millis();
    }
}

void wait(unsigned long t) {
    unsigned long ms = millis();
    while (!isPaused && ms + t > millis()) {
        if (Serial.available()) {
            railStepper.getCommand();
        }
    }
}

void timer() {
    if (!isTiming) {
        start = micros();
    } else {
        Serial.println("RT: " + (String)(micros() - start));
    }
    isTiming = !isTiming;
}

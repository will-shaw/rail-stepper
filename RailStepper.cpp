
#include "RailStepper.hpp"

double RailStepper::version = 1.3;

RailStepper::RailStepper(int bumperPin, int triggerPin, 
                         int motorPin1, int motorPin2, 
                         int motorPin3, int motorPin4) {
    this->bumperPin = bumperPin;
    this->triggerPin = triggerPin;
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
    digitalWrite(motorPin1,LOW);
    digitalWrite(motorPin2,LOW);
    digitalWrite(motorPin3,LOW);
    digitalWrite(motorPin4,LOW);
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
    
    RailStepper(motorPin1, motorPin2, motorPin3, motorPin4);
}

RailStepper::RailStepper(int motorPin1, int motorPin2, 
                         int motorPin3, int motorPin4) {
    int bumperPin;
    int triggerPin;
    
    int stabilityDelay;
    int stepSize;
    int railLength;
    int direction = -1;
    this->motorPin1 = motorPin1;
    this->motorPin2 = motorPin2;
    this->motorPin3 = motorPin3;
    this->motorPin4 = motorPin4;
}

void RailStepper::setStepSize(int stepSize) {
    this->stepSize = stepSize;
}

int RailStepper::getStepSize() {
    return this->stepSize;
}

void RailStepper::setPaused(bool value) {
    this->isPaused = !this->isPaused;
}

bool RailStepper::getPaused() {
    return this->isPaused;
}

void RailStepper::setDirection(int direction) {
    this->direction = direction;
}

int RailStepper::getDirection() {
    return this->direction;
}

int RailStepper::shoot() {
    digitalWrite(triggerPin, HIGH);
    delay(200);
    digitalWrite(triggerPin, LOW);
    frame++;
    return frame;
}

void RailStepper::checkSerial() {
    String in = "";
    while (Serial.available() > 0) {
        in += (char) Serial.read();
        delay(2);
    }
    
    if (in.length() > 1) {
        if (in.startsWith("r")) {
            direction = in.substring(1).toInt();
            printRotation();
            stepSize = 0 - stepSize;
            printMove();
        
        } else if (in.startsWith("m")) {
            stepSize = direction * (in.substring(1).toInt());
            printMove();
        
        } else if (in.startsWith("d")) {
            shootDelay = (in.substring(1).toInt());
            printDelay();
        }
        
    } else if (in.equals(">")) {
        step();
        
    } else if (in.equals("<")) {
        step();
        
    } else if (in.equals("p")) {
        isPaused = !isPaused;
        printPause();
        
    } else if (in.equals("r")) {
        direction = direction == 1 ? -1 : 1;
        printRotation();
        stepSize = 0 - stepSize;
        printMove();
        
    } else if (in.equals("s")) {
        frame = 0;
        stepCount = 0;
        Serial.println(!isPaused ? "Frames Reset" : "Active");
        Serial.println(direction == 1 ? "A-C move to start" : "C move to start");
        moveToStart();
        //isPaused = !isPaused;
        
    } else if (in.equals("a")) {
        moveToStart();

    } else if (in.equals("i")) {
        printMove();
        printDelay();
        printPause();
        printFrame();
        printStepCount();
        printRotation();
        
    } else if (!in.equals("")){
        Serial.println("Unknown Command");
        
    }
    in = "";
}

void RailStepper::moveToStart() {
    Serial.println('o');
    while (analogRead(bumperPin) <= 100 && !isStopByte()) {
        step();
    }
    Serial.println('w');
    isStart = true;
    stepCount = 0;
}


boolean RailStepper::isStopByte() {
    if (Serial.available() > 0) {
        if ((char) Serial.peek() == 's' || (char) Serial.peek() == 'p') {
            Serial.println((char)Serial.peek());
            return true; 
        }
    }
    return false;
}

void RailStepper::moveToCenter() {
    Serial.println('c');
    int steps = (stepCount / 2), count = 0;
    while (isReset && !isStopByte() && count < steps) {
        step();
        count++;
    }
    isReset = false;
    isPaused = true;
    printPause();
}

void RailStepper::step() {
    switch (this->direction) {
        case this->CLOCKWISE:
            digitalWrite(motorPin1, HIGH);
            delay(stepDelay);
            digitalWrite(motorPin4, LOW);
            delay(stepDelay);
            digitalWrite(motorPin2, HIGH);
            delay(stepDelay);
            digitalWrite(motorPin1, LOW);
            delay(stepDelay);
            digitalWrite(motorPin3, HIGH);
            delay(stepDelay);
            digitalWrite(motorPin2, LOW);
            delay(stepDelay);
            digitalWrite(motorPin4, HIGH);
            delay(stepDelay);
            digitalWrite(motorPin3, LOW);
            delay(stepDelay);
            break;

        case this->ANTI_CLOCKWISE:
            digitalWrite(motorPin4, HIGH);
            delay(stepDelay);
            digitalWrite(motorPin1, LOW);
            delay(stepDelay);
            digitalWrite(motorPin3, HIGH);
            delay(stepDelay);
            digitalWrite(motorPin4, LOW);
            delay(stepDelay);
            digitalWrite(motorPin2, HIGH);
            delay(stepDelay);
            digitalWrite(motorPin3, LOW);
            delay(stepDelay);
            digitalWrite(motorPin1, HIGH);
            delay(stepDelay);
            digitalWrite(motorPin2, LOW);
            delay(stepDelay);
        break;
    }
}

void RailStepper::move(int m) {
    m = abs(m);
    Serial.println('j');
    while ((isStart || analogRead(bumperPin) <= 100) && m > 0 && !isStopByte()) {
        step();
        m--;
        stepCount++;
        if (stepCount > 10) {
          isStart = false;
        }
    }
    if (analogRead(bumperPin) > 100) {
      Serial.println("Stopped");
      isReset = true;
      moveToCenter();
    }
}

void RailStepper::printMove() {
    Serial.println("m" + (String) stepSize);
}

void RailStepper::printDelay() {
    Serial.println("d" + (String) stepDelay);
}

void RailStepper::printPause() {
    Serial.println(isPaused ? "p" : "j");
}

void RailStepper::printRotation() {
    Serial.println(direction == 1 ? "c" : "ac");
}

void RailStepper::printFrame() {
    Serial.println("f" + (String) frame);
}

void RailStepper::printStepCount() {
    Serial.println("s:" + (String) stepCount);
}

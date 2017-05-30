#ifndef RAILSTEPPER_H_
#define RAILSTEPPER_H_

#include "Arduino.h"

class RailStepper {

  private:
    int motorPin1;
    int motorPin2;
    int motorPin3;
    int motorPin4;
    int bumperPin;
    int triggerPin;
    int railLength;
    
    int stabilityDelay = 1000;
    int stepSize = 1;
    int stepDelay = 2;
    int stepCount = 0;
    int shootDelay = 5000;
    int direction = 1;
    bool isStart = false;
    bool isReset = false;
    bool isPaused = true;
    int frame = 0;
    
  public:
    static double version;
    static const int CLOCKWISE = 1;
    static const int ANTI_CLOCKWISE = -1;
    static const bool PAUSED = true;
    static const bool RUNNING = false;

    RailStepper(int bumperPin, int triggerPin, 
                int motorPin1, int motorPin2, 
                int motorPin3, int motorPin4);    
    
    RailStepper(int motorPin1, int motorPin2, 
                int motorPin3, int motorPin4);

    void step();
    void setStepSize(int stepSize);
    int getStepSize();    
    void setPaused(bool value);
    bool getPaused();
    void setDirection(int value);
    int getDirection();
    int shoot();
    void checkSerial();
    int getShootDelay();
    void setShootDelay(int shootDelay);
    void moveToStart();
    void moveToCenter();
    boolean isStopByte();
    void move(int m);
    void getCommand();

    void printMove();
    
    void printDelay();
    
    void printPause();
    
    void printRotation();
    
    void printFrame();
    
    void printStepCount();
    
};

#endif // RAILSTEPPER_H_

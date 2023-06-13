#pragma once
#include <Servo.h>
#include <stb_brain.h>
#include <stb_servo_shared.h>


#define SERVO_CNT 4 // PWM Anschlüsse


class STB_SERVO {

    private:
    Servo servos[SERVO_CNT]; 


    const int ServoPins[SERVO_CNT] = {9, 6, 5, 3};
    bool getBufferValues(STB_BRAIN &Brain, int nrValues,  int &values);
    
    public:
    STB_SERVO();    
    
    //static bool ServoInit();
    bool evaluateCmds(STB_BRAIN &Brain);
    void moveSingleServo(int stripValue, int pos);
    //void moveServo(int stripNo);
};
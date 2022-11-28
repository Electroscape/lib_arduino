#pragma once
#include <Arduino.h>
#include "stb_buzzer_cmds.h"

#define BuzzerMaxStages 3

class STB_BUZZER {
    private:
    // maybe a better way to organize just copying 
    struct BuzzerParsStruct {
        unsigned int freq[BuzzerMaxStages] = {0};
        unsigned long onTime[BuzzerMaxStages] = {0};
        unsigned long offTime[BuzzerMaxStages] = {0};
        unsigned long timeStamp = millis();
        int stage = -1;
        int pin;
    };

    BuzzerParsStruct BuzzerPars;

    public:
    STB_BUZZER();
    void init(int pwmPin);
    void update();
    void triggerBuzzer();
    void buzzerStop();
};
#pragma once
#include <Arduino.h>
#include "stb_buzzer_cmds.h"

#define BuzzerMaxStages 3

class STB_BUZZER {
    // maybe a better way to organize just copying 
    unsigned int buzzerFreq[BuzzerMaxStages] = {0};
    unsigned long buzzerOn[BuzzerMaxStages] = {0};
    unsigned long buzzerOff[BuzzerMaxStages] = {0};
    unsigned long buzzerTimeStamp = millis();
    int buzzerStage = -1;
    int buzzerPin;

    void init(int pwmPin);
    void buzzerUpdate();
    void buzzerReset();
};
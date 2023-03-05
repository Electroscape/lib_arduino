/**
 * @file stb_shared.h
 * @author Martin Pek (martin.pek@web.de)
 * @brief used to store variables used by both Mother and Brain
 * @version 0.1
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <Arduino.h>

// defines the passing of settings, particularily how many settings the brain receive and store
#define SETTINGS_CNT 8
// col in the settings matrix, 
#define SETTINGS_PARAMS 5


enum cmdFlags {
    ledFlag=1,
    rfidFlag=2,
    oledFlag=4,
    keypadFlag=8,
    servoFlag = 16,
    // update this everytime you add flags, only count the flags
};


enum settingCmds {
    ledCount,
    rfidAmount,
    /* 
    Dots -> NEO_RGB, 
    5V WS2812B strips -> NEO_GRB
    */
    ledClrOrder,
    // to iterate over settings
    amountOfSettings
};


// global keywords being used
class KeywordsList {
    public:
    static const String flagKeyword;
    static const String settingKeyword;
    static const String beginKeyword;
    static const String keypadKeyword;
    static const String rfidKeyword;
    static const String ledKeyword;
    // pollStr is used to poll, get information from the brain
    static const String pollStr;
    // pushStr is used to send messaged to the brain
    static const String pushStr;
    static const String ACK;
    // static const String NACK;
    static const String delimiter;
    static const String eof;
    static const String oledHKeyword;
    static const String ServoKeyword;
};

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
#define SETTINGS_PARAMS 3


enum cmdFlags {
    ledFlag,
    rfidFlag,
    oledFlag,
    keypadFlag,
    // update this everytime you add flags, only count the flags
    amountOfFlags = 4
};


enum settingCmds {
    ledCount,
    rfidAmount,
    // update this everytime you add flags, only count the cmds
    amountOfSettings = 2
};


// global keywords being used
class KeywordsList {
    public:
    static const String flagKeyword;
    static const String endFlagKeyword;
    static const String settingKeyword;
    static const String keypadKeyword;
    static const String endSettingKeyword;
    static const String rfidKeyword;
    static const String ledKeyword;
    static const String pollStr;
    static const String ACK;
    static const String delimiter;
    static const String relayKeyword;
    static const String NACK;
    static const String eof;
};

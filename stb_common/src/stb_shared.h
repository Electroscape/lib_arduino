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


// defines the passing of settings, particularily how many settings the brain receive and store
#define SETTINGS_CNT 8
// col in the settings matrix, 
#define SETTINGS_PARAMS 3


enum cmdFlags {
    ledFlag,
    rfidFlag,
    oledFlag,
    // update this everytime you add flags, only count the flags
    amountOfFlags = 3
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
    char flagKeyword[7] = "!Flag_";
    char endFlagKeyword[10] = "!FlagsEnd";
    char settingKeyword[6] = "!Sett";
    char endSettingKeyword[9] = "!SettEnd";
    char rfidKeyword[7] = "!RFID_";
    char ledKeyword[6] = "!LED_";
    char pollStr[6] = "!Poll";
    char ACK[7] = "!ACK";
    char delimiter[2] = "_";
    char relayKeyword[7] = "!Relay";
    char NACK[6] = "!NACK";
    char eof[6] = "!EOF";
};

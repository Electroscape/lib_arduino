/**
 * @file stb_mother.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "stb_mother.h"


STB_MOTHER::STB_MOTHER(/* args */) {}

STB_MOTHER::~STB_MOTHER() {}


/**
 * @brief sends a single flag to given brain, 
 * send flagsCompleted after all flags are send
 * @param STB 
 * @param brainNo 
 * @param cmdFlag 
 * @param status 
 */
void STB_MOTHER::setFlag(STB STB, int brainNo, cmdFlags cmdFlag, bool status) {

    char msg[16] = "";
    strcpy(msg, keyWords.flagKeyword);
    char noString[3];
    sprintf(noString, "%d", cmdFlag);
    strcat(msg, noString);
    strcat(msg, "_");

    if (status) {
        strcat(msg, "1");
    } else {
        strcat(msg, "0");
    }

    while (true) {
        STB.rs485setSlaveAsTgt(brainNo);
        STB.rs485AddToBuffer(msg);
        if (STB.rs485SendBuffer(true)) {
            //delay(500);
            return;
        }
        wdt_reset();
        // delay(20);
    }
}


/**
 * @brief sends a signal to the brain that no more flags 
 * are going to be send and the setup can continue
 * @param STB 
 * @param brainNo 
 */
void STB_MOTHER::flagsCompleted(STB STB, int brainNo) {
    while (true) {
        STB.rs485setSlaveAsTgt(brainNo);
        STB.rs485AddToBuffer(keyWords.endFlagKeyword);
        if (STB.rs485SendBuffer(true)) {
            return;
        }
        wdt_reset();
    }
}


int STB_MOTHER::rs485getPolledSlave() {
    return polledSlave;
}


int STB_MOTHER::rs485getSlaveCnt() {
    return slaveCount;
}


/**
 * @brief defines how many slaves are being polled
 * @param count 
 */
void STB_MOTHER::rs485SetSlaveCount(int count) {
    slaveCount = count;
}


#ifdef true
/**
 * @brief sets Master along with relay initialisation
 */
void STB_MOTHER::rs485SetToMaster() {
    // TODO: may need to add a parameter for pins and initvalues
    defaultOled.setScrollMode(SCROLL_MODE_AUTO);
    int relayPins[8] = {0,1,2,3,4,5,6,7};
    int relayInitVals[8] = {1,1,1,1,1,1,1,1};
    relayInit(motherRelay, relayPins, relayInitVals);
}
#endif 


/**
 * @brief sends a given setting to the brain
 * @param STB 
 * @param brainNo 
 * @param setting 
 * @param values 
 */
void STB_MOTHER::sendSetting(STB STB, int brainNo, settingCmds setting, int values[], int amountOfValues) {
 
    char msg[32] = "";
    char noString[8];

    strcpy(msg, keyWords.settingKeyword);
    strcat(msg, "_");
    sprintf(noString, "%d", setting);
    strcat(msg, noString);

    for (int index; index < amountOfValues; index++) {
        strcat(msg, "_");
        sprintf(noString, "%d", values[index]);
        strcat(msg, noString);
    }

    while (true) {
        STB.rs485setSlaveAsTgt(brainNo);
        STB.rs485AddToBuffer(msg);
        if (STB.rs485SendBuffer(true)) {
            return;
        }
        wdt_reset();
    }
}


/**
 * @brief sends a message to the brain to no longer wait for settings
 * @param STB 
 * @param brainNo 
 */
void STB_MOTHER::settingsCompleted(STB STB, int brainNo) {
    while (true) {
        STB.rs485setSlaveAsTgt(brainNo);
        STB.rs485AddToBuffer(keyWords.endSettingKeyword);
        if (STB.rs485SendBuffer(true)) {
            return;
        }
        wdt_reset();
    }
}


/**
 * @brief initializes the given relay along with init states
 * @param relay (PCF8574) relay instance
 * @param pins (int) pin numbers
 * @param initvals (int) init value
 * @param amount (int) amount of relays to be initialized
 * @return bool
 */
bool STB_MOTHER::relayInit(PCF8574 &relay, int pins[], int initvals[], int amount) {
    String relayString = String(RELAY_I2C_ADD, HEX);
    relayString.toUpperCase();
    // dbgln("relayinit on " + relayString); 

    relay.begin(RELAY_I2C_ADD);
    
    for (int i = 0; i < amount; i++) {
        relay.pinMode(pins[i], OUTPUT);
        relay.digitalWrite(pins[i], initvals[i]);
        // dbg("Relay ["); dbg(String(pins[i])); dbg("] set to "); dbgln(String(initvals[i]));
    }
    delay(500);
    return true;
}
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

void STB_MOTHER::begin() {
    STB_.begin();
    STB_.i2cScanner();
}


/**
 * @brief sends all flags to be active in as binary, any flag not added to flags is inactive
 * @param STB 
 * @param brainNo 
 * @param flags being a sum of multiple flags, those flags add up binary due to 2^n numbering
 * @param status 
 */
void STB_MOTHER::setFlags(int brainNo, int flags) {

    char msg[16] = "";
    strcpy(msg, KeywordsList::flagKeyword.c_str());
    char noString[8];
    sprintf(noString, "%d", flags);
    strcat(msg, noString);

    setSlaveAsTgt(brainNo);
    STB_.rs485AddToBuffer(msg);

    while (true) {
        if (STB_.rs485SendBuffer(true)) {
            return;
        }
        delay(5);
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
 * @brief polls the bus slaves and forwards the input to cmdInterpreter
 */
void STB_MOTHER::rs485PerformPoll() {
    polledSlave++;
    if (polledSlave >= slaveCount) {
        polledSlave = 0;
    }

    char message[16];
    // there should not be other data left here anyways, alternativle use strCat
    strcpy(message,  KeywordsList::pollStr.c_str());
    char slaveNoStr[3];
    sprintf(slaveNoStr, "%i", polledSlave);
    strcat(message, slaveNoStr);
    
    STB_.rs485AddToBuffer(message);
    STB_.rs485SendBuffer();
    STB_.rs485Receive();

    // needs to be modified
    /*
    if (strlen(STB_.rcvdPtr) > 0) {
       dbgln(STB_.rcvdPtr);
    }
    */
}


/**
 * @brief send the given message to the designated slave, by default this will be the polledSlave
 * @param slaveNo 
 * @param message 
 * @return if message got acknowled
 */
bool STB_MOTHER::sendCmdToSlave(char* message, int slaveNo) {
    delay(1);
    if (slaveNo < 0) {
        slaveNo = polledSlave;
    }

    setSlaveAsTgt(slaveNo);
    STB_.rs485AddToBuffer(String(message));
    while (!STB_.rs485SendBuffer(true)) {
        wdt_reset();
        // maybe spamming too much aint teh best idea
        delay(5);
    }
    return true;
};


/**
 * @brief WIP adds string of slave to be polled into the buffer
 * @param slaveNo 
 */
void STB_MOTHER::setSlaveAsTgt(int slaveNo) {

    char message[16];
    // there should not be other data left here anyways, alternativle use strCat
    strcpy(message,  KeywordsList::pushStr.c_str());
    char slaveNoStr[3];
    sprintf(slaveNoStr, "%i", slaveNo);
    strcat(message, slaveNoStr);
    
    STB_.rs485AddToBuffer(message);
};


/**
 * @brief defines how many slaves are being polled
 * @param count 
 */
void STB_MOTHER::rs485SetSlaveCount(int count) {
    slaveCount = count;
}


/**
 * @brief sends a given setting to the brain
 * @param STB 
 * @param brainNo 
 * @param setting 
 * @param values 
 */
void STB_MOTHER::sendSetting(int brainNo, settingCmds setting, int values[], int amountOfValues) {
 
    char msg[32] = "";
    char noString[8];

    strcpy(msg, KeywordsList::settingKeyword.c_str());
    strcat(msg, "_");
    sprintf(noString, "%d", setting);
    strcat(msg, noString);

    for (int index=0; index < amountOfValues; index++) {
        strcat(msg, "_");
        sprintf(noString, "%d", values[index]);
        strcat(msg, noString);
    }

    setSlaveAsTgt(brainNo);
    STB_.rs485AddToBuffer(msg);

    while (true) {
        if (STB_.rs485SendBuffer(true)) {
            return;
        }
        delay(5);
        wdt_reset();
    }
}


/**
 * @brief signals brain to into the mainloop after all flags/settings have been passed
 * @param STB 
 * @param brainNo 
 */
void STB_MOTHER::setupComplete(int brainNo) {

    setSlaveAsTgt(brainNo);
    STB_.rs485AddToBuffer(KeywordsList::beginKeyword);

    while (true) {
        if (STB_.rs485SendBuffer(true)) {
            return;
        }
        delay(5);
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
bool STB_MOTHER::relayInit(int pins[], int initvals[], int amount) {
    
    String relayString = String(RELAY_I2C_ADD, HEX);
    relayString.toUpperCase();
    STB_OLED::writeHeadline(&STB_.defaultOled, "Relay " + relayString);
    motherRelay.begin(RELAY_I2C_ADD);
    String pinStr = ""; 
    String valueStr = "";

    for (int i = 0; i < amount; i++) {
        motherRelay.pinMode(pins[i], OUTPUT);
        motherRelay.digitalWrite(pins[i], initvals[i]);
        pinStr += String(pins[i]) + " ";
        valueStr += String(initvals[i]) + " ";
        // dbg("Relay ["); dbg(String(pins[i])); dbg("] set to "); dbgln(String(initvals[i]));
    }
    STB_OLED::writeToLine(&STB_.defaultOled, 2, pinStr);
    STB_OLED::writeToLine(&STB_.defaultOled, 3, valueStr);
    delay(500);
    return true;
}
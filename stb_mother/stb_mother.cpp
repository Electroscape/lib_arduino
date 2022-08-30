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
 * @brief sends a single flag to given brain, 
 * send flagsCompleted after all flags are send
 * @param STB 
 * @param brainNo 
 * @param cmdFlag 
 * @param status 
 */
void STB_MOTHER::setFlag(int brainNo, cmdFlags cmdFlag, bool status) {

    char msg[16] = "";
    strcpy(msg, KeywordsList::flagKeyword.c_str());
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
        rs485setSlaveAsTgt(brainNo);
        STB_.rs485AddToBuffer(msg);
        if (STB_.rs485SendBuffer(true)) {
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
void STB_MOTHER::flagsCompleted(int brainNo) {
    while (true) {
        rs485setSlaveAsTgt(brainNo);
        STB_.rs485AddToBuffer(KeywordsList::endFlagKeyword);
        if (STB_.rs485SendBuffer(true)) {
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
 * @brief polls the bus slaves and forwards the input to cmdInterpreter
 */
void STB_MOTHER::rs485PerformPoll() {
    polledSlave++;
    if (polledSlave >= slaveCount) {
        polledSlave = 0;
    }

    rs485setSlaveAsTgt(polledSlave);
    // TODO: reenable this once STB cleanup proceeded 
    /*
    rs485Write();
    rs485Receive();

    // needs to be modified
    if (strlen(rcvd) > 0) {
        // TODO: new input receive here
        dbgln(rcvd);
    }
    */
}


/**
 * @brief send the given message to the designated slave
 * @param slaveNo 
 * @param message 
 * @return if message got acknowled
 */
bool STB_MOTHER::rs485SendCmdToSlave(int slaveNo, char* message) {
    /*
    TODO: reenable once STB clean proceede

    rs485setSlaveAsTgt(slaveNo);
    // newline is handled by teh aboive so nothing to worry
    strcat(bufferOut, message);
    dbgln("cmd to slave");
    dbgln(bufferOut);
    rs485SendBuffer();
    */
    return true;
};


/**
 * @brief WIP adds string of slave to be polled into the buffer
 * @param slaveNo 
 */
void STB_MOTHER::rs485setSlaveAsTgt(int slaveNo) {
    /*
    TODO: reenable once STB clean proceeded 
    // there should not be other data left here anyways, alternativle use strCat
    strcpy(bufferOut, pollStr);
    char slaveNoStr[3];
    sprintf(slaveNoStr, "%i", slaveNo);
    strcat(bufferOut, slaveNoStr);
    strcat(bufferOut, "\n");
    // this will put the slaveStr in beginning of the buffer
    */
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

    for (int index; index < amountOfValues; index++) {
        strcat(msg, "_");
        sprintf(noString, "%d", values[index]);
        strcat(msg, noString);
    }

    while (true) {
        rs485setSlaveAsTgt(brainNo);
        STB_.rs485AddToBuffer(msg);
        if (STB_.rs485SendBuffer(true)) {
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
void STB_MOTHER::settingsCompleted(int brainNo) {
    while (true) {
        rs485setSlaveAsTgt(brainNo);
        STB_.rs485AddToBuffer(KeywordsList::endSettingKeyword);
        if (STB_.rs485SendBuffer(true)) {
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
    STB_OLED::writeHeadline(&STB_.defaultOled, "Relay" + relayString);
    
    relay.begin(RELAY_I2C_ADD);

    for (int i = 0; i < amount; i++) {
        relay.pinMode(pins[i], OUTPUT);
        relay.digitalWrite(pins[i], initvals[i]);
        STB_.defaultOled.print(String(pins[i]));
        STB_.defaultOled.setCol(STB_.defaultOled.col() + 1);
        STB_.defaultOled.print(String(initvals[i]));
        STB_.defaultOled.setCol(STB_.defaultOled.col() - 1);
        // dbg("Relay ["); dbg(String(pins[i])); dbg("] set to "); dbgln(String(initvals[i]));
    }
    STB_.defaultOled.setCol(STB_.defaultOled.col() + 1);
    STB_.defaultOled.println();
    delay(500);
    return true;
}
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
 * @brief sends a single flag to given brain, send flagsCompleted after all flags are send
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
 * @brief sends a signal to the brain that no more flags are going to be send
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
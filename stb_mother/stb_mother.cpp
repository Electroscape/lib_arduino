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

bool STB_MOTHER::setFlag(STB STB, int brainNo, cmdFlags cmdFlag, bool status) {

    STB.rs485setSlaveAsTgt(brainNo);
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
    
    STB.rs485AddToBuffer(msg);
    return STB.rs485SendBuffer(true);
}
/**
 * @file stb_mother_ledCmds.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief  
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright Copyright (c) 2022
 * TODO: 
 * - implement adding the clr to the msg as a fnc
*/

#include "stb_mother_ledCmds.h"


constexpr int LED_CMDS::clrWhite[3];
constexpr int LED_CMDS::clrRed[3];
constexpr int LED_CMDS::clrGreen[3];
constexpr int LED_CMDS::clrBlack[3];



/**
 * @brief sets the given brain to the given clr
 * @param STB_MOTHER 
 * @param brainNo 
 * @param clr 
 * @param ledCnt default is all Leds
 */
void LED_CMDS::setToClr(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness, int ledCnt) {
    char msg[32] = "";
    char noString[3] = "";
    sprintf(noString, "%i", ledCmds::setAll);
    
    strcpy(msg, KeywordsList::ledKeyword.c_str());
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    // repeat function ... pack this into a fnc
    for (int i=0; i<3; i++) {
        char intStr[3];
        itoa((clr[i] * brightness) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }

    Mother.sendCmdToSlave(msg, brainNo);
}


/**
 * @brief  sets a single given pixel to 
 * @param Mother 
 * @param pixel 
 * @param clr 
 * @param brightness 
 * @param brainNo 
*/
void LED_CMDS::setPixelToClr(STB_MOTHER &Mother, int pixel ,const int clr[3], int brightness, int brainNo) {
    if (brainNo < 0) {
        brainNo = Mother.rs485getPolledSlave();
    }

    char msg[32] = "";
    char noString[3] = "";
    sprintf(noString, "%i", ledCmds::setPixel);

    strcpy(msg, KeywordsList::ledKeyword.c_str());
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    sprintf(noString, "%i", pixel);
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());

    // repeat function ... pack this into a fnc
    for (int i=0; i<3; i++) {
        char intStr[3];
        itoa((clr[i] * brightness) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }

    Mother.sendCmdToSlave(msg, brainNo);
}

/**
 * @brief switches off the LEDs on the given brain
 * @param STB_MOTHER
 * @param brainNo 
 * @param ledCnt default is all Leds
 */
void LED_CMDS::turnOff(STB_MOTHER &Mother, int brainNo, int ledCnt) {
    setToClr(Mother, brainNo, clrBlack, int(100), ledCnt);
}
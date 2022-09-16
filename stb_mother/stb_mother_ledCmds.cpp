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
void LED_CMDS::setToClr(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness=100, int ledCnt) {
    
    char msg[32] = "";
    strcpy(msg, KeywordsList::ledKeyword.c_str());

    for (int i=0; i<3; i++) {
        char intStr[3];
        itoa((clr[i] * brightness) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }
    
    Mother.STB_.clearBuffer();
    Mother.setSlaveAsTgt(brainNo);
    Mother.STB_.rs485AddToBuffer(msg);

    while (true) {
        if (Mother.STB_.rs485SendBuffer(true)) { return; }
        wdt_reset();
        delay(5);
    }
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
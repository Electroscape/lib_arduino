#include "stb_mother_ledCmds.h"


constexpr int LED_CMDS::clrWhite[3];
constexpr int LED_CMDS::clrRed[3];
constexpr int LED_CMDS::clrGreen[3];
constexpr int LED_CMDS::clrBlack[3];
constexpr char LED_CMDS::ledCmdString[16];


/**
 * @brief sets the given brain to the given clr
 * @param STB 
 * @param brainNo 
 * @param clr 
 * @param ledCnt default is all Leds
 */
void LED_CMDS::setToClr(STB &STB, int brainNo, const int clr[3], int brightness=100, int ledCnt) {
    
    char msg[32] = "";
    strcpy(msg, ledCmdString);

    for (int i=0; i<3; i++) {
        char intStr[3];
        itoa((clr[i] * brightness) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }

    while (true) {
        STB.rs485AddToBuffer(msg);
        if (STB.rs485SendBuffer(true)) { break;}
        wdt_reset();
    }
}


/**
 * @brief switches off the LEDs on the given brain
 * @param STB 
 * @param brainNo 
 * @param ledCnt default is all Leds
 */
void LED_CMDS::turnOff(STB &STB, int brainNo, int ledCnt) {
    setToClr(STB, brainNo, clrBlack, int(100), ledCnt);
}
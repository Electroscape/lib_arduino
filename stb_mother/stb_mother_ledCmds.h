#pragma once
#include <stb_led_shared.h>
#include <stb_mother.h>

class LED_CMDS {
    
    private:
    // static void clrToString(char *&addToMsg, const int clr[3], int brightness);
    
    public:
    static constexpr int clrWhite[3] = {255,255,255};
    static constexpr int clrRed[3] = {255,0,0};
    static constexpr int clrGreen[3] = {0,255,0};
    static constexpr int clrBlack[3] = {0,0,0};
    static constexpr int clrBlue[3] = {0,0,255};
    static constexpr int clrYellow[3] = {255,255,0};

    /*could possibly just pass the Mother class once? 
    brainNo as optional param? otherwise take the currently polled slave
    */
    static void setAllStripsToClr(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness=100, int ledCnt=-1);
    static void setStripToClr(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness, int stripNo);
    static void turnOff(STB_MOTHER &Mother, int brainNo, int ledCnt=-1);
};
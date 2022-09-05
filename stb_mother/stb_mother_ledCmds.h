#pragma once
#include <stb_mother.h>
#include <avr/wdt.h>

class LED_CMDS {
    
    private:
    
    public:
    static constexpr int clrWhite[3] = {255,255,255};
    static constexpr int clrRed[3] = {255,0,0};
    static constexpr int clrGreen[3] = {0,255,0};
    static constexpr int clrBlack[3] = {0,0,0};

    /*could possibly just pass the Mother class once? 
    brainNo as optional param? otherwise take the currently polled slave
    */
    static void setToClr(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness=100, int ledCnt=-1);
    static void turnOff(STB_MOTHER &Mother, int brainNo, int ledCnt=-1);
};
#pragma once
#include <stb_led_shared.h>
#include <stb_mother.h>
#include <avr/wdt.h>

class LED_CMDS {
    
    private:
    static bool hasKeyword(STB_MOTHER &Mother);
    
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
    static void setPixelToClr(STB_MOTHER &Mother, int pixel ,const int clr[3], int brainNo=-1)
};
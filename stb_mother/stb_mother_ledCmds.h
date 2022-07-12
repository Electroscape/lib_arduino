#pragma once
#include <stb_common.h>
#include <avr/wdt.h>

class LED_CMDS {
    
    private:
    // still fixed to one brain, I need to make sure SendCmdToSlave works first
    static constexpr char ledCmdString[16] = "!Poll0\n!LED_\0";
    
    public:
    static constexpr int clrWhite[3] = {255,255,255};
    static constexpr int clrRed[3] = {255,0,0};
    static constexpr int clrGreen[3] = {0,255,0};
    static constexpr int clrBlack[3] = {0,0,0};

    static void setToClr(STB &STB, int brainNo, const int clr[3], int brightness=100, int ledCnt=-1);
    static void turnOff(STB &STB, int brainNo, int ledCnt=-1);
};
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
    static void setPixelToClr(STB_MOTHER &Mother, int pixel ,const int clr[3], int brightness=100, int brainNo=-1);
    static void running(STB_MOTHER &Mother, int brainNo,const int clr[3],int runTime= 1200,int actLED=2);
    static void runningPWM(STB_MOTHER &Mother, int brainNo,const int clr[3],int runTime= 1200,int actLED=2);
    static void blinking(STB_MOTHER &Mother, int brainNo,const int clr1[3],const int clr2[3],int blinkTime1=500, int blinkTime2=500);
    static void dimming(STB_MOTHER &Mother, int brainNo, int dimmTime = 2000, int endBrightness = 0);
    
};
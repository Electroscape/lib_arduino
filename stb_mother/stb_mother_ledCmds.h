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
    static constexpr int clrPurple[3] = {136,0,255};

    /*could possibly just pass the Mother class once? 
    brainNo as optional param? otherwise take the currently polled slave
    */
    static void setAllStripsToClr(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness=100, int ledCnt=-1);
    static void setStripToClr(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness, int stripNo);
    static void turnOff(STB_MOTHER &Mother, int brainNo, int ledCnt=-1);
    static void setLEDToClr(STB_MOTHER &Mother,int brainNo , const int clr[3], int brightness,int stripNo, int LED_Nr);
    static void running(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness, int runTime, int usedLED, int stripNo,int pauseTime = 0);
    static void runningPWM(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness, int runTime = 1200, int usedLED = 2,int pauseTime = 0);
    static void blinking(STB_MOTHER &Mother, int brainNo, const int clr1[3], const int clr2[3], int blinkTime1, int blinkTime2, int brightness1, int brightness2, int stripNo);
    static void fade2color(STB_MOTHER &Mother, int brainNo, const int clr1[3], int brightness1, const int clr2[3], int brightness2, int runTime, int stripNo);
};

enum PWM {
    out,
    set1, // PWM 0
    set2, // PWM 1
    set1_2, // PWM 0 +1
    set3, // PWM 2
    set1_3, // PWM 0+2
    set2_3, // PWM 1+2
    set1_2_3, // PWM 0+1+2
    set4,   // PWM 3
    set1_4, // PWM 0+3
    set2_4, // PWM 1+3
    set1_2_4, // PWM 0+1+3
    set3_4, // PWM 2+3
    set1_3_4, // PWM 0+2+3
    set2_3_4, // PWM 1+2+3
    set1_2_3_4, // PWM 0+1+2+3
};
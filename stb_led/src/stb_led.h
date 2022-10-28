#pragma once
#include <Adafruit_NeoPixel.h>
#include <stb_brain.h>
#include <stb_shared.h>
#include <stb_led_shared.h>


#define STRIPE_CNT 4
// due to complile time requirements we cannot dynamically allocate the amount
#define LED_MAX_CNT 60

class STB_LED {
    private:
    const int16_t ledPins[STRIPE_CNT] = {9, 6, 5, 3};
    // amount of leds that are used, note we need to declare at compile time hence this may be less
    uint16_t activeLeds[STRIPE_CNT] = {0};

    bool getClrsFromBuffer(STB_BRAIN &Brain, long int &setClr);
    bool getBufferValues(STB_BRAIN &Brain, int nrValues,  int &values);

    void enableStrip0(uint32_t clrOrder=NEO_BRG);
    void enableStrip1(uint32_t clrOrder=NEO_BRG);
    void enableStrip2(uint32_t clrOrder=NEO_BRG);
    void enableStrip3(uint32_t clrOrder=NEO_BRG);


    public:
    Adafruit_NeoPixel Strips[STRIPE_CNT];
    STB_LED();    
    long clrBlack =STB_LED::Strips->Color(0,0,0);
    
    long int color1;// color1 @running & @blinking
    long int color2;// color2 @running & @blinking
    int runTime;    // runtime @running
    int actLED;     // number of used LEDS @running & @blinking
    int blinkTime1; // blinking time color1 @blinking
    int blinkTime2; // blinking time color2 @blinking
    int lightState[4] = {0,0,0,0};    // state of PWM, define with ledCmds 
    unsigned long lightTiming[4];
    uint16_t LED_ON[4]; //@running
    unsigned long deltaTime[4] = {0,0,0,0}; // time delta for timed LED function

    bool ledInit(int settings[SETTINGS_CNT][SETTINGS_PARAMS], uint32_t clrOrder=NEO_RGB, int clkSpeed=NEO_KHZ800);
    void setStripToClr(int stripNo, long int clr, bool show=true);
    // void setStripToClrs(int stripNo, uint32_t clrs[], int size);
    void setAllStripsToClr(long int clr);
    bool evaluateCmds(STB_BRAIN &Brain);
    void setLEDToClr(int stripNo,uint16_t ledNr, long int clr) ;
    void running(int stripNo, long int clr, int runTime=1200, int actLED=2);
    void runningPWM(long int clr, int runTime=1200, uint16_t actLED=4);
    void blinking(int stripNo, const int clr1, const int clr2, int blinkTime1=500, int blinkTime2=500);
    void dimming(int stripNo, int dimmTime = 2000, int endBrightness = 0);
    void LEDloop(STB_BRAIN &Brain);
};
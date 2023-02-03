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
    void lightDog();
    unsigned long lightDogTimer = millis();

    void enableStrip0(uint32_t clrOrder=NEO_BRG);
    void enableStrip1(uint32_t clrOrder=NEO_BRG);
    void enableStrip2(uint32_t clrOrder=NEO_BRG);
    void enableStrip3(uint32_t clrOrder=NEO_BRG);


    public:
    Adafruit_NeoPixel Strips[STRIPE_CNT];
    STB_LED();    
    long clrBlack =STB_LED::Strips->Color(0,0,0);
   
    struct TimeVarsStruct
    {
        int usedLED;                    // how many LEDs are used        
        int blinkTime[2];               // blinking time for color 1 and 2
        int pauseTime;  
        int effektTime;                 
        int lightMode = -1;            // operation mode of PWM, defined with ledCmds 
        unsigned long modeDuration;      // timestamp for light functions switching in next loop =0 -> reset timer
        unsigned long deltaTime = 0;    // delat in time for next action
        uint16_t LED_ON = 0;            // current active LED
        uint32_t color[2];              // second color for blinking
    };
    
    TimeVarsStruct TimeVars[STRIPE_CNT];

    

    bool ledInit(int settings[SETTINGS_CNT][SETTINGS_PARAMS], uint32_t clrOrder=NEO_RGB, int clkSpeed=NEO_KHZ800);
    void setStripToClr(int stripNo, long int clr, bool show=true);
    // void setStripToClrs(int stripNo, uint32_t clrs[], int size);
    void setAllStripsToClr(long int clr);
    bool evaluateCmds(STB_BRAIN &Brain);
    void setLEDToClr(int stripNo,uint16_t ledNr, long int clr) ;
    void running(int stripNo);
    void runningPWM();
    void blinking(int stripNo);
    void fade2color(int stripNo);
    void LEDloop(STB_BRAIN &Brain);
};
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
    
    // Variables used for light effects like running light
    // pack LEDvariables in Matrix
    //uint32_t colorLEDLoop[STRIPE_CNT][2];                      // color storage for functions
    //uint32_t color1[STRIPE_CNT];                         // color1 @running & @blinking
   // uint32_t color2[STRIPE_CNT];                         // color2 @running & @blinking
    int TimeVarsTest[STRIPE_CNT][12];                    // diferent variables for timed LED functions 1: actLED/actPWM(used nr of PWM or LEDS on a PWM) 3&4:runTime (time the function or effect is running, two for e.g. blinking) 4
    //int usedLED[STRIPE_CNT];                              // number of used LEDS @running & @blinking
    //int runTime[STRIPE_CNT];                             // runtime @running
    //int blinkTime1[STRIPE_CNT];                         // blinking time color1 @blinking
    //int blinkTime2[STRIPE_CNT];                         // blinking time color2 @blinking
    //int lightState[STRIPE_CNT] = {-1,-1,-1,-1};          // operation mode of PWM, defined with ledCmds 
    //uint8_t brightness[STRIPE_CNT];
    int endbrightness[STRIPE_CNT];
    //int pauseTime [STRIPE_CNT];
    //unsigned long lightTiming[STRIPE_CNT];
    //uint16_t LED_ON[STRIPE_CNT]= {0,0,0,0};            // used for the running light
    //unsigned long deltaTime[STRIPE_CNT] = {0,0,0,0}; // time delta for timed LED function
   
    struct TimeVarsStruct
    {
        int usedLED;  
        int blinkTime[2];   
        int pauseTime;
        int effektTime;
        int lightState = -1;
        unsigned long lightTiming;   
        unsigned long deltaTime = 0;
        uint16_t LED_ON = 0;
        uint32_t color[2];
    };
    
    TimeVarsStruct TimeVars[4];

    

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
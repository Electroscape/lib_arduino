#pragma once
#include <Adafruit_NeoPixel.h>
#include <stb_shared.h>


#define STRIPE_CNT 1
// due to complile time requirements we cannot dynamically allocate the amount
#define ledMaxCnt 60

class STB_LED {
    public:
    Adafruit_NeoPixel Strips[STRIPE_CNT];
    const int16_t ledPins[STRIPE_CNT] = {9}; //, 6, 5, 3
    //const uint16_t ledMaxCnt= 70;
    STB_LED();
    void begin();
    void enableStrip0();
    static bool ledInit(Adafruit_NeoPixel neopixels[], int settings[SETTINGS_CNT][SETTINGS_PARAMS], uint32_t clrOrder=NEO_RGB, int clkSpeed=NEO_KHZ800);
    static void setStripToClr(Adafruit_NeoPixel &neopixel, long int clr);
    static void setStripToClrs(Adafruit_NeoPixel &neopixel, uint32_t clrs[], int size);
    static void setAllStripsToClr(Adafruit_NeoPixel neopixels[],  int stripeCount, long int clr);
};
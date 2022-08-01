#pragma once
#include <Adafruit_NeoPixel.h>
#include <stb_shared.h>


#define STRIPE_CNT 4
// due to complile time requirements we cannot dynamically allocate the amount
#define LED_MAX_CNT 60

class STB_LED {
    private:
    const int16_t ledPins[STRIPE_CNT] = {9, 6, 5, 3};
    // amount of leds that are used, note we need to declare at compile time hence this may be less
    uint16_t activeLeds[STRIPE_CNT] = {0};

    void enableStrip0();
    void enableStrip1();
    void enableStrip2();
    void enableStrip3();


    public:
    Adafruit_NeoPixel Strips[STRIPE_CNT];
    STB_LED();

    bool ledInit(int settings[SETTINGS_CNT][SETTINGS_PARAMS], uint32_t clrOrder=NEO_RGB, int clkSpeed=NEO_KHZ800);
    void setStripToClr(int stripNo, long int clr);
    void setStripToClrs(int stripNo, uint32_t clrs[], int size);
    void setAllStripsToClr(long int clr);
};
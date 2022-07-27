#pragma once
#include <Adafruit_NeoPixel.h>
#include <stb_shared.h>


#define STRIPE_CNT 4


class STB_LED {
    public:
    static bool ledInit(Adafruit_NeoPixel neopixels[], int settings[SETTINGS_CNT][SETTINGS_PARAMS], uint32_t clrOrder=NEO_RGB, int clkSpeed=NEO_KHZ800);
    static void setStripToClr(Adafruit_NeoPixel &neopixel, long int clr);
    static void setStripToClrs(Adafruit_NeoPixel &neopixel, uint32_t clrs[], int size);
    static void setAllStripsToClr(Adafruit_NeoPixel neopixels[],  int stripeCount, long int clr);
};
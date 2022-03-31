#pragma once
#include <Adafruit_NeoPixel.h>

class STB_LED {
    public:
    static bool ledInit(Adafruit_NeoPixel neopixels[], int ledCnts[], int pins[], uint32_t clrOrder=NEO_RGB, int clkSpeed=NEO_KHZ800);

};
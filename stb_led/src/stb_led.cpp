#include "stb_led.h"

/**
 * @brief 
 * 
 * @param neopixel 
 * @param ledCnts 
 * @param pins 
 * @param clrOrder defaults to NEO_RGB
 * @param clkSpeed defaults to NEO_KHZ800
 * @return true 
 * @return false 
 */
bool STB_LED::ledInit(Adafruit_NeoPixel neopixels[], int ledCnts[], int pins[], uint32_t clrOrder, int clkSpeed) {
    for (size_t i=0; i<sizeof(neopixels); i++) {
        neopixels[i] = Adafruit_NeoPixel(ledCnts[i], pins[i], (clrOrder + clkSpeed));
        neopixels[i].begin();
        for (int ledNr=0; ledNr<ledCnts[i]; ledNr++) {
            neopixels[i].setPixelColor(ledNr, neopixels[i].Color(0, 0, 0));
            neopixels[i].show();
            delay(10);
        }
    }
    return true;
}
/**
 * @file stb_led.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-01
 * 
 */

#include "stb_led.h"

/**
 * @brief 
 * 
 * @param neopixel array of type Adafruit_NeoPixel
 * @param ledCnts int array delcaring how many leds are on said strip
 * @param pins  int array with the pins to be used
 * @param clrOrder defaults to NEO_RGB
 * @param clkSpeed defaults to NEO_KHZ800
 * @return true 
 * @return false 
 */
bool STB_LED::ledInit(Adafruit_NeoPixel neopixels[], int ledCnts[], int pins[], uint32_t clrOrder, int clkSpeed) {
    for (size_t i=0; i<sizeof(neopixels); i++) {
        neopixels[i] = Adafruit_NeoPixel(ledCnts[i], pins[i], (clrOrder + clkSpeed));
        neopixels[i].begin();
        STB_LED::setAllStripsToClr(neopixels, neopixels[i].Color(0, 0, 0));
    }
    delay(50);
    return true;
}

/**
 * @brief 
 * 
 * @param neopixel single led instance
 * @param clr 
 */
void STB_LED::setStripToClr(Adafruit_NeoPixel &neopixel, long int clr) {
    for (uint16_t ledNr=0; ledNr<neopixel.numPixels(); ledNr++) {
        neopixel.setPixelColor(ledNr, clr);
        neopixel.show();
    }
    delay(10);
}

/**
 * @brief 
 * 
 * @param neopixels array of led instances
 * @param clr 
 */
void STB_LED::setAllStripsToClr(Adafruit_NeoPixel neopixels[], long int clr) {
    for (size_t i=0; i<sizeof(neopixels); i++) {
        STB_LED::setStripToClr(neopixels[i], clr);
    }
}
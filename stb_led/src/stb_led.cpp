/**
 * @file stb_led.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-01
 * 
 */

#include "stb_led.h"


STB_LED::STB_LED() {};


void STB_LED::begin() {
    delay(200);
    Serial.println("STB_LED::begin");
    uint32_t clrOrder=NEO_BRG; 
    int clkSpeed=NEO_KHZ800;
    int i = 0;
    Strips[i] = Adafruit_NeoPixel(ledMaxCnt, ledPins[i], (NEO_BRG + NEO_KHZ800));
    Strips[i].begin();
    delay(200);
    Strips[i].setPixelColor(0, Strips[0].Color(0, 0, 255));
    Strips[i].setPixelColor(1, Strips[0].Color(0, 0, 0));
    Strips[i].setPixelColor(2, Strips[0].Color(59, 0, 0));
    Strips[i].show();
    delay(100);

};


void STB_LED::enableStrip0() {
    delay(10);
    Serial.println("STB_LED::begin");
    uint32_t clrOrder=NEO_BRG; 
    int clkSpeed=NEO_KHZ800;
    int i = 1;
    Strips[i] = Adafruit_NeoPixel((uint16_t) 3, ledPins[i], (NEO_BRG + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
    Strips[i].setPixelColor(0, Strips[0].Color(90, 0, 0));
    Strips[i].setPixelColor(2, Strips[0].Color(0, 50, 0));
    Strips[i].setPixelColor(1, Strips[0].Color(50, 0, 99));
    Strips[i].show();
}

/**
 * @brief 
 * 
 * @param neopixel array of type Adafruit_NeoPixel
 * @param ledCnts int array delcaring how many leds are on said strip
 * @param pins  int array with the pins to be used
 * @param clrOrder defaults to NEO_BRG
 * @param clkSpeed defaults to NEO_KHZ800
 * @return true 
 * @return false 
 */
bool STB_LED::ledInit(Adafruit_NeoPixel neopixels[], int settings[SETTINGS_CNT][SETTINGS_PARAMS], uint32_t clrOrder, int clkSpeed) {
    
    Serial.print(F("\n\nLED init ..."));
    uint16_t ledCnts[STRIPE_CNT] = {0};

    int row = 0;
    while (settings[row][0] >= 0 && row < SETTINGS_CNT) {
        if (settings[row][0] == settingCmds::ledCount) {
            int stripeNo = settings[row][1];
            // safety check otherwise we go out of index
            if (stripeNo < 0 || stripeNo >= STRIPE_CNT) { continue; }
            ledCnts[stripeNo] = (uint16_t) settings[row][2];
            Serial.println(String(ledCnts[stripeNo]));
        }
        row++;
    }
    
    delay(10);

    int16_t ledPins[STRIPE_CNT] = {9}; // ,  6, 5, 3

    for (int i=0; i<STRIPE_CNT; i++) {
        if (ledCnts[i] <= 0) { continue; }
        Serial.print("led count for ");
        Serial.print(i);
        Serial.print(" is: ");
        Serial.println(ledCnts[i]);
        Serial.print("ledpin is");
        Serial.println(ledPins[i]);
        neopixels[0] = Adafruit_NeoPixel((uint16_t) 2, (int16_t) 9, (clrOrder + clkSpeed));
        neopixels[0].begin();
        STB_LED::setAllStripsToClr(neopixels, 1,  neopixels[i].Color(0, 0, 0));
    }
    delay(40);
    Serial.println(F(" successful"));
    delay(10);

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
 * @param neopixel 
 * @param clrs 
 */
void STB_LED::setStripToClrs(Adafruit_NeoPixel &neopixel, uint32_t  clrs[], int size) {
    for (int ledNr=0; ledNr<size; ledNr++) {
        neopixel.setPixelColor(ledNr, clrs[ledNr]);
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
void STB_LED::setAllStripsToClr(Adafruit_NeoPixel neopixels[], int stripeCount, long int clr) {
    for (int i=0; i<stripeCount; i++) {
        setStripToClr(neopixels[i], clr);
    }
}
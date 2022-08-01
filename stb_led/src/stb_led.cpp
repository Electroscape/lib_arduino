/**
 * @file stb_led.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-01
 * 
 */

#include "stb_led.h"


/*
    Todo:
    cleanups
    passing of clr order and freq to enableStrips 
*/


STB_LED::STB_LED() {};


void STB_LED::begin() {
    delay(200);
};


void STB_LED::enableStrip0() {
    delay(10);
    Serial.println("STB_LED::enableStrip0");
    uint32_t clrOrder=NEO_BRG; 
    int clkSpeed=NEO_KHZ800;
    int i = 0;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (NEO_BRG + NEO_KHZ800));
    Strips[i].begin();
    /*
    Strips[i].setPixelColor(0, Strips[0].Color(0,0,0));
    Strips[i].setPixelColor(2, Strips[0].Color(50, 150, 50));
    Strips[i].setPixelColor(1, Strips[0].Color(100, 0, 0));
    Strips[i].show();
    */
    delay(10);
}


void STB_LED::enableStrip1() {
    delay(10);
    Serial.println("STB_LED::enableStrip1");
    uint32_t clrOrder=NEO_BRG; 
    int clkSpeed=NEO_KHZ800;
    int i = 1;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (NEO_BRG + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip2() {
    delay(10);
    Serial.println("STB_LED::enableStrip2");
    uint32_t clrOrder=NEO_BRG; 
    int clkSpeed=NEO_KHZ800;
    int i = 2;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (NEO_BRG + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip3() {
    delay(10);
    Serial.println("STB_LED::enableStrip3");
    uint32_t clrOrder=NEO_BRG; 
    int clkSpeed=NEO_KHZ800;
    int i = 3;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (NEO_BRG + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
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
 * Todo: prevent this from possibly being stuck when wrong data is rcvd
 */
bool STB_LED::ledInit(int settings[SETTINGS_CNT][SETTINGS_PARAMS], uint32_t clrOrder, int clkSpeed) {
    
    Serial.print(F("\n\nLED init ..."));
 
    int row = 0;
    while (row < SETTINGS_CNT) {
        if (settings[row][0] == settingCmds::ledCount) {
            int stripeNo = settings[row][1];
            // safety checks otherwise we go out of index
            if (stripeNo < 0 || stripeNo >= STRIPE_CNT) { continue; }
            // if (LED_MAX_CNT < settings[row][2]) { continue; }

            activeLeds[stripeNo] = (uint16_t) settings[row][2];
        // check & exit if we ran through all passed settings
        } else if (settings[row][0] < 0) {
            break;
        }
        row++;
    }


    for (int i=0; i<STRIPE_CNT; i++) {
        if (activeLeds[i] <= 0) { continue; }
        switch (i) {
            case 0: enableStrip0(); break;
            case 1: enableStrip1(); break;
            case 2: enableStrip2(); break;
            case 3: enableStrip3(); break;
        }
    }
    setAllStripsToClr(Strips[0].Color(0,0,0));
    Serial.println(F(" successful"));

    Serial.print("enabled so many leds: ");
    Serial.print(String(activeLeds[0]));
    Serial.print(" ");
    Serial.print(String(activeLeds[1]));
    Serial.print(" ");
    Serial.print(String(activeLeds[2]));
    Serial.print(" ");
    Serial.print(String(activeLeds[3]));

    return true;
}

/**
 * @brief 
 * @param stripNo index from 0-3
 * @param clr 
 */
void STB_LED::setStripToClr(int stripNo, long int clr) {
    Serial.println(String(activeLeds[stripNo]));
    if ((int) activeLeds[stripNo] <= 0) { return; }
    Serial.print("setting stripNo ");
    Serial.println(String(stripNo));
    for (uint16_t ledNr=0; ledNr<activeLeds[stripNo]; ledNr++) {
        Strips[stripNo].setPixelColor(ledNr, clr);
    }
    Strips[stripNo].show();
}


/**
 * @brief 
 * 
 * @param neopixel 
 * @param clrs 
 */
void STB_LED::setStripToClrs(int stripNo, uint32_t  clrs[], int size) {
    for (int ledNr=0; ledNr<size; ledNr++) {
        Strips[stripNo].setPixelColor(ledNr, clrs[ledNr]);
        Strips[stripNo].show();
    }
    delay(10);
}
/**
 * @brief 
 * 
 * @param neopixels array of led instances
 * @param clr 
 */
void STB_LED::setAllStripsToClr(long int clr) {
    Serial.println("STB_LED::setAllStripsToClr");
    for (int i=0; i<STRIPE_CNT; i++) {
        // the check if the strip is activated is done in the following fnc
        setStripToClr(i, clr);
    }
}
#pragma once
#include <Adafruit_NeoPixel.h>
#include <stb_shared.h>


#define STRIPE_CNT 4

#define RFID_1_LED_PIN          9     /* Per Konvention ist dies RFID-Port 1                                */
#define RFID_2_LED_PIN          6     /* Per Konvention ist dies RFID-Port 2                                */
#define RFID_3_LED_PIN          5     /* Per Konvention ist dies RFID-Port 3                                */
#define RFID_4_LED_PIN          3     /* Per Konvention ist dies RFID-Port 4   */


class STB_LED {
    public:
    static int constexpr ledPins[STRIPE_CNT] = {RFID_1_LED_PIN, RFID_2_LED_PIN, RFID_3_LED_PIN, RFID_4_LED_PIN};
    static bool ledInit(Adafruit_NeoPixel neopixels[], int settings[SETTINGS_CNT][SETTINGS_PARAMS], uint32_t clrOrder=NEO_RGB, int clkSpeed=NEO_KHZ800);
    static void setStripToClr(Adafruit_NeoPixel &neopixel, long int clr);
    static void setStripToClrs(Adafruit_NeoPixel &neopixel, uint32_t clrs[], int size);
    static void setAllStripsToClr(Adafruit_NeoPixel neopixels[],  int stripeCount, long int clr);
};
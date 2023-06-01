#include <stb_led.h>


#define STRIPE_CNT             1
// doublecheck if this is inside the stb_led
#define RFID_1_LED_PIN          9 

int ledCnts[STRIPE_CNT] = {100};
int ledPins[STRIPE_CNT] = {LED_PIN_1};

Adafruit_NeoPixel LED_Strips[STRIPE_CNT];
const long int someClr = LED_Strips[0].Color(0,255,0);

char ledKeyword[] = "!LED";

void setup() {
    STB_LED::ledInit(LED_Strips, 1, ledCnts, ledPins, NEO_BRG);
}

void loop() {
    STB_LED::setAllStripsToClr(LED_Strips, STRIPE_CNT, someClr);
    STB_LED::setStripToClr(LED_Strips[0], someClr);
}
/**
 * @file stb_keypad.h
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "stb_keypad.h"
 

STB_KEYPAD::STB_KEYPAD() {
    Keypad_I2C Keypad(makeKeymap(KeypadKeys), KeypadRowPins, KeypadColPins, rows, cols, 0x39, PCF8574_MODE);
}

bool STB_KEYPAD::init(int keypadAddr) {
    // 
    return true;
}



/*

void (*listener)(char)
bool keypadInit() {
    Keypad.addEventListener(keypadEvent);  // Event Listener erstellen
    Keypad.begin(makeKeymap(KeypadKeys));
    Keypad.setHoldTime(5000);
    Keypad.setDebounceTime(20);
    return true;
}
*/
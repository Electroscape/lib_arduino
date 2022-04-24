#pragma once

#include <Keypad_I2C.h>

class STB_KEYPAD {
    private:
    static constexpr byte rows = 4;
    static constexpr byte cols = 3;
    static constexpr byte KEYPAD_CODE_LENGTH = 4;
    static constexpr byte KEYPAD_CODE_LENGTH_MAX = 7;

    static constexpr char KeypadKeys[rows][cols] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
    };

    byte KeypadRowPins[rows] = {1, 6, 5, 3};  // Zeilen  - Messleitungen
    byte KeypadColPins[cols] = {2, 0, 4};     // Spalten - Steuerleitungen (abwechselnd HIGH)

    public:
    Keypad_I2C* Keypad = nullptr;
    STB_KEYPAD();
    bool init(int keypadAddr);
};
#pragma once

#include <Arduino.h>
#include <PCF8574.h> /* I2C Port Expander  */
#include <Wire.h>    /* I2C library */

// Defined by hardware
#define MAX_CTRL_PIN A0
#define MAX485_WRITE HIGH
#define MAX485_READ LOW

#ifndef RELAY_I2C_ADD
    #define RELAY_I2C_ADD 0x3F
#endif 

class STB {
    public:
    static void printWithHeader(String message, String source=String("SYS"));
    // void heartbeat();
    // void softwareReset();
    // bool i2cScanner();
    static bool brainSerialInit();
    static bool relay_init(Expander_PCF8574 relay, int pins[], int initvals[], int amount=8);
};





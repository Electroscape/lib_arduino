#pragma once

#include <Arduino.h>
#include <Keypad.h>
#include <Keypad_I2C.h>
#include <Password.h>
#include <Wire.h>

// Defined by hardware
#define MAX_CTRL_PIN A0
#define MAX485_WRITE HIGH
#define MAX485_READ LOW

#ifndef RELAY_I2C_ADD
    #define RELAY_I2C_ADD 0x3F
#endif 

#define i2cClkSpeed 100000

class STB {
    private:
    static bool serialInit();
    static void printInfo();
    static void printI2cDeviceName(int deviceNo);
    
    public:
    static void begin();
    static void printWithHeader(String message, String source=String("SYS"));
    static void printSetupEnd();

    static bool i2cScanner();
    static void softwareReset();
    
    static bool relayInit(PCF8574 &relay, int pins[], int initvals[], int amount=8);
};




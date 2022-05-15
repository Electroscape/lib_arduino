#pragma once

#include <Arduino.h>
#include <PCF8574.h> /* I2C Port Expander  */
#include <Wire.h>    /* I2C library */
#include <stb_oled.h>

// Defined by hardware
#define MAX_CTRL_PIN A0
#define MAX485_WRITE HIGH
#define MAX485_READ LOW

#ifndef RELAY_I2C_ADD
    #define RELAY_I2C_ADD 0x3F
#endif 

#define i2cClkSpeed 100000
#define rs485timeout 50

class STB {
    private:
    static bool serialInit();
    static void printInfo();
    bool rs485PollingCheck(String message);
    void printI2cDeviceName(int deviceNo);
    bool isMaster = false;
    cstring messageBuffer = "";
    
    public:
    SSD1306AsciiWire defaultOled;
    STB();
    void begin();
    static void printWithHeader(String message, String source=String("SYS"));
    void printSetupEnd();
    void dbg(String message);
    void dbgln(String message);
    void RS485SetToMaster();
    bool rs485Write(String message);
    bool i2cScanner();
    static void softwareReset();
    
    bool relayInit(PCF8574 &relay, int pins[], int initvals[], int amount=8);
};

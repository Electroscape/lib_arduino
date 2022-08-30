#pragma once

#include <Arduino.h>
#include <PCF8574.h> /* I2C Port Expander  */
#include <Wire.h>    /* I2C library */
#include <stb_oled.h>
#include <stb_shared.h>

// Defined by hardware
#define MAX_CTRL_PIN A0
#define MAX485_WRITE HIGH
#define MAX485_READ LOW

#define i2cClkSpeed 100000
#define bufferSize 64

class STB {

    private:
    long rs485timeout = 10;
    // start at -1 to start with slave 0 since we do the increment in the beginning
    // maybe change this name to keep things less confusing sine mother uses slaveStr
    unsigned long maxPollingWait = 300;
    // time the master waits for  the slave to respond
    unsigned long maxResponseTime = 300;
    char rcvd[bufferSize] = "";
    char bufferOut[bufferSize] = "";

    bool serialInit();
    void printInfo();
    void rs485Write();
    bool rs485Receive();
    void printI2cDeviceName(int deviceNo);
    
    public:
    SSD1306AsciiWire defaultOled;
    
    char* rcvdPtr;

    STB();
    void begin();
    void printWithHeader(String message, String source=String("SYS"));
    void printSetupEnd();
    void dbg(String message);
    void dbgln(String message);
    
    
    // ideally it would be an array of which slave number are active...
    bool rs485AddToBuffer(String message);
    
    void rs485SendAck();
    bool rs485SendBuffer(bool isCmd=false);
    bool rs485RcvdNextLn();
    bool i2cScanner();
    void softwareReset();
};

#pragma once

#include <Arduino.h>
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
    unsigned int writedelay = 50;
    // start at -1 to start with slave 0 since we do the increment in the beginning
    // maybe change this name to keep things less confusing sine mother uses slaveStr
    // time the master waits for  the slave to respond
    // this needs to be higher for the mother?
    unsigned long maxResponseTime = 120;
    char rcvd[bufferSize] = "";
    char bufferOut[bufferSize] = "";
    bool bufferSplit = false;

    bool serialInit();
    void printInfo();
    void rs485Write(bool persistent=false);
    void printI2cDeviceName(int deviceNo);
    
    public:
    bool rs485Receive();
    unsigned long maxPollingWait = 30;
    SSD1306AsciiWire defaultOled;
    
    char* rcvdPtr;

    STB();
    void begin();
    void printWithHeader(String message, String source=String("SYS"));
    void printSetupEnd();
    void dbg(String message);
    void dbgln(String message);
    
    
    // ideally it would be an array of which slave number are active...
    bool rs485AddToBuffer(char* message);
    void clearBuffer();
    
    bool checkAck();
    void rs485SendAck();
    bool rs485SendBuffer(bool isCmd=false);
    bool rs485RcvdNextLn();
    bool i2cScanner();
    static void softwareReset();
};

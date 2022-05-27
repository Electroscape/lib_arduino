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
#define slaveCount 8
#define bufferSize 64

class STB {
    private:
    long rs485timeout = 10;
    bool serialInit();
    static void printInfo();
    bool rs485PollingCheck(String message);
    void printI2cDeviceName(int deviceNo);
    void cmdInterpreter(char *rcvd, int slaveNo);
    bool isMaster = false;
    int slaveAddr = 0;
    // instead of generating better store that String to look for
    String slavePollStr = "MASTER";
    char eof[5] = "!EOF";
    char delimiter[2] = "_";
    char relayKeyword[7] = "!Relay";
    unsigned long maxPollingWait = 300;
    // time the master waits for  the slave to respond
    unsigned long maxResponseTime = 20;
    char buffer[bufferSize] = "";
    
    public:
    SSD1306AsciiWire defaultOled;
    // may move to
    PCF8574 motherRelay;

    STB();
    void begin();
    static void printWithHeader(String message, String source=String("SYS"));
    void printSetupEnd();
    void dbg(String message);
    void dbgln(String message);
    void rs485SetToMaster();
    void rs485SetSlaveAddr(int no);
    void rs485PerformPoll();
    bool rs485AddToBuffer(String message);
    bool rs485Write(String message);
    bool rs485PollingCheck();
    bool rs485SendRelayCmd(int relayNo, int value);
    bool i2cScanner();
    static void softwareReset();
    
    bool relayInit(PCF8574 &relay, int pins[], int initvals[], int amount=8);
};

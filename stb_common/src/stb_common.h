#pragma once

#include <Arduino.h>
#include <PCF8574.h> /* I2C Port Expander  */
#include <Wire.h>    /* I2C library */
#include <stb_oled.h>

// Defined by hardware
#define MAX_CTRL_PIN A0
#define MAX485_WRITE HIGH
#define MAX485_READ LOW

#define i2cClkSpeed 100000
#define bufferSize 64

class STB {

    private:

    long rs485timeout = 10;
    int slaveAddr = 0;
    int polledSlave = -1;
    int slaveCount = 8;
    // start at -1 to start with slave 0 since we do the increment in the beginning
    
    // instead of generating better store that String to look for
    char pollStr[6] = "!Poll";
    // maybe change this name to keep things less confusing sine mother uses slaveStr
    char delimiter[2] = "_";
    char relayKeyword[7] = "!Relay";
    char NACK[6] = "!NACK";
    char eof[6] = "!EOF";
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
    

    char ACK[7] = "!ACK";
    char slavePollStr[8] = "!Poll9";
    char* rcvdPtr;

    STB();
    void begin();
    static void printWithHeader(String message, String source=String("SYS"));
    void printSetupEnd();
    void dbg(String message);
    void dbgln(String message);
    int rs485getPolledSlave();
    
    void rs485SetSlaveAddr(int no);
    // ideally it would be an array of which slave number are active...
    void rs485SetSlaveCount(int count);
    bool rs485AddToBuffer(String message);
    bool rs485SlaveRespond();
    void rs485SendAck();
    bool rs485SendBuffer(bool isCmd=false);
    bool rs485PollingCheck();
    bool rs485RcvdNextLn();
    bool i2cScanner();
    static void softwareReset();
};

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
#define bufferSize 64

class STB {

    private:
    int slaveCount = 8;
    long rs485timeout = 10;
    bool isMaster = false;
    int slaveAddr = 0;
    // start at -1 to start with slave 0 since we do the increment in the beginning
    int polledSlave = -1;
    // instead of generating better store that String to look for
    char pollStr[6] = "!Poll";
    // maybe change this name to keep things less confusing sine mother uses slaveStr
    char delimiter[2] = "_";
    char relayKeyword[7] = "!Relay";
    char NACK[6] = "!NACK";
    char eof[6] = "!EOF\0";
    unsigned long maxPollingWait = 300;
    // time the master waits for  the slave to respond
    unsigned long maxResponseTime = 300;
    char rcvd[bufferSize] = "";
    char bufferOut[bufferSize] = "";

    bool serialInit();
    static void printInfo();
    void rs485Write();
    bool rs485Receive();
    void rs485setSlaveAsTgt(int slaveNo);
    void printI2cDeviceName(int deviceNo);
    
    public:
    SSD1306AsciiWire defaultOled;
    PCF8574 motherRelay;

    char ACK[7] = "!ACK\0";
    char slavePollStr[8] = "!Poll9\0";
    char rcvdLn[bufferSize] = "";
    char* rcvdPtr;

    STB();
    void begin();
    static void printWithHeader(String message, String source=String("SYS"));
    void printSetupEnd();
    void dbg(String message);
    void dbgln(String message);
    void rs485SetToMaster();
    void rs485SetSlaveAddr(int no);
    // ideally it would be an array of which slave number are active...
    void rs485SetSlaveCount(int count);
    void rs485PerformPoll();
    bool rs485AddToBuffer(String message);
    bool rs485SlaveRespond();
    void rs485SendAck();
    bool rs485SendBuffer(bool isCmd=false);
    bool rs485PollingCheck();
    bool rs485RcvdNextLn();
    bool rs485SendCmdToSlave(int slaveNo, char* message);
    bool i2cScanner();
    static void softwareReset();
    
    bool relayInit(PCF8574 &relay, int pins[], int initvals[], int amount=8);
};

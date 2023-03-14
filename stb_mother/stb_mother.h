#pragma once

#include <stb_common.h>
#include <stb_shared.h>
#include <avr/wdt.h>
#include <PCF8574.h> /* I2C Port Expander  */

#ifndef RELAY_I2C_ADD
    #define RELAY_I2C_ADD 0x3F
#endif 
#define RESET_I2C_ADD 0x3D
#define RESEND_ATTEMPTS 30

class STB_MOTHER
{
    private:
        int slaveCount = 8;
        int polledSlave = -1;
    public:
        PCF8574 motherRelay;
        STB STB_;
        STB_MOTHER();
        ~STB_MOTHER();

        void begin();

        // settings & flags
        void setFlags(int brainNo, int flags);
        int getPolledSlave();
        int getSlaveCnt();
        void rs485PerformPoll(int brainNo=-1);
        bool sendCmdToSlave(char* message, int slaveNo=-1);
        void setSlaveAsTgt(int slaveNo);
        void rs485SetSlaveCount(int count);
    
        
        void sendSetting(int brainNo, settingCmds setting, int values[], int amountOfValues);
        void setupComplete(int brainNo);

        bool relayInit(int pins[], int initvals[], int amount=8);

        // linking functions to shorten code
        void dbgln(String message) { STB_.dbgln(message); };
        void dbg(String message) { STB_.dbg(message); };
        void oledClear() {STB_.defaultOled.clear();};
        void addToBuffer(char* message) {STB_.rs485AddToBuffer(message);};
        void sendAck() {STB_.rs485SendAck();};
        bool nextRcvdLn() {return STB_.rs485RcvdNextLn();};
};



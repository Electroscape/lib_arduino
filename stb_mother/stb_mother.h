#pragma once

#include <stb_common.h>
#include <stb_shared.h>
#include <avr/wdt.h>
#include <PCF8574.h> /* I2C Port Expander  */

#ifndef RELAY_I2C_ADD
    #define RELAY_I2C_ADD 0x3F
#endif 

class STB_MOTHER
{
    private:
        STB STB_;
        int slaveCount = 8;
        int polledSlave = -1;
        PCF8574 motherRelay;
    public:
        STB_MOTHER();
        ~STB_MOTHER();

        // settings & flags
        void setFlag(int brainNo, cmdFlags cmdFlag, bool status);
        void flagsCompleted(int brainNo);
        int rs485getPolledSlave();
        int rs485getSlaveCnt();
        void rs485PerformPoll();
        bool rs485SendCmdToSlave(int slaveNo, char* message);
        void rs485setSlaveAsTgt(int slaveNo);
        void rs485SetSlaveCount(int count);
        
        // TODO: Evaluate if this may simply be moved into the constructor
        // void rs485SetToMaster(int count);
        
        void sendSetting(int brainNo, settingCmds setting, int values[], int amountOfValues);
        void settingsCompleted(int brainNo);

        bool relayInit(PCF8574 &relay, int pins[], int initvals[], int amount=8);

        void dbgln(String message) { STB_.dbgln(message); };
        void dbg(String message) { STB_.dbg(message); };
};



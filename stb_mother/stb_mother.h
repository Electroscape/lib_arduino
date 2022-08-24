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
        int slaveCount = 8;
        int polledSlave = -1;
        PCF8574 motherRelay;
    public:
        STB_MOTHER();
        ~STB_MOTHER();
        KeywordsList keyWords;

        // settings & flags
        void setFlag(STB STB, int brainNo, cmdFlags cmdFlag, bool status);
        void flagsCompleted(STB STB, int brainNo);
        int rs485getPolledSlave();
        int rs485getSlaveCnt();
        void rs485PerformPoll();
        bool rs485SendCmdToSlave(int slaveNo, char* message);
        void rs485setSlaveAsTgt(int slaveNo);
        void rs485SetSlaveCount(int count);
        
        // TODO: Evaluate if this may simply be moved into the constructor
        // void rs485SetToMaster(int count);
        
        void sendSetting(STB STB, int brainNo, settingCmds setting, int values[], int amountOfValues);
        void settingsCompleted(STB STB, int brainNo);

        bool relayInit(PCF8574 &relay, int pins[], int initvals[], int amount=8);
};



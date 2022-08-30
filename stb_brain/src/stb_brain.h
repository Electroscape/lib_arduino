#pragma once

#include <stb_common.h>
#include <stb_shared.h>
#include <avr/wdt.h>


class STB_BRAIN {
    private:
        /* data */
    public:
        STB STB_;
        int slaveAddr = 0;
        char slavePollStr[8] = "!Poll9";
        // by default everything is enabled
        void rs485SetSlaveAddr(int no);
        bool rs485PollingCheck();
        int flags[cmdFlags::amountOfFlags] = {0};
        int settings[SETTINGS_CNT][SETTINGS_PARAMS] = {};
        void receiveFlags();
        void receiveSettings();
        bool rs485SlaveRespond();
        void softwareReset() {STB_.softwareReset(); };
        
        void dbgln(String message) { STB_.dbgln(message); };
        void dbg(String message) { STB_.dbg(message); };
        
        
        STB_BRAIN();
        ~STB_BRAIN();
};
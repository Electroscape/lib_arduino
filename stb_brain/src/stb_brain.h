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
        void setSlaveAddr(int no);
        bool pollingCheck();
        int flags[cmdFlags::amountOfFlags] = {0};
        int settings[SETTINGS_CNT][SETTINGS_PARAMS] = {};
        void receiveFlags();
        void receiveSettings();
        bool slaveRespond();
        void softwareReset() {STB_.softwareReset(); };
        
        // linking functions to shorten code
        void dbgln(String message) { STB_.dbgln(message); };
        void dbg(String message) { STB_.dbg(message); };
        void oledClear() {STB_.defaultOled.clear();};
        void addToBuffer(String message) {STB_.rs485AddToBuffer(message);};
        void sendAck() {STB_.rs485SendAck();};
        bool nextRcvdLn() {return STB_.rs485RcvdNextLn();};

        STB_BRAIN();
        ~STB_BRAIN();
};
#pragma once

#include <stb_common.h>
#include <stb_shared.h>
#include <avr/wdt.h>


class STB_BRAIN {
    private:
        /* data */
    public:
        STB STB_;
        bool outgoingCmd = false;
        int slaveAddr = 0;
        char slavePollStr[7];
        char slavePushStr[7];
        // by default everything is enabled
        void begin();
        void setSlaveAddr(int no);
        int pollingCheck();
        int flags = 0;
        int settings[SETTINGS_CNT][SETTINGS_PARAMS] = {};
        bool receiveFlags();
        bool receiveSetting();
        bool slaveRespond();
        void softwareReset() {STB_.softwareReset(); };
        
        // linking functions to shorten code
        void dbgln(String message) { STB_.dbgln(message); };
        void dbg(String message) { STB_.dbg(message); };
        void oledClear() {STB_.defaultOled.clear();};
        void addToBuffer(String message, bool containsCmd=false);
        void sendAck() {STB_.rs485SendAck();};
        bool nextRcvdLn() {return STB_.rs485RcvdNextLn();};

        STB_BRAIN();
        ~STB_BRAIN();
};
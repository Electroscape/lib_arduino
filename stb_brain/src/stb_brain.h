#pragma once

#include <stb_common.h>
#include <stb_shared.h>
#include <avr/wdt.h>


class STB_BRAIN {
    private:
        STB STB;
        /* data */
    public:
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
        STB_BRAIN(/* args */);
        ~STB_BRAIN();
};
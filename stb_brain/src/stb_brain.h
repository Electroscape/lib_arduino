#pragma once

#include <stb_common.h>
#include <stb_shared.h>
#include <avr/wdt.h>


class STB_BRAIN {
    private:
        KeywordsList Keywords;
        /* data */
    public:
        // by default everything is enabled
        int flags[cmdFlags::amountOfFlags] = {0};
        int settings[SETTINGS_CNT][SETTINGS_PARAMS] = {};
        void receiveFlags(STB STB);
        void receiveSettings(STB STB);
        STB_BRAIN(/* args */);
        ~STB_BRAIN();
};
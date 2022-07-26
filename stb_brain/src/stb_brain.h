#pragma once

#include <stb_common.h>
#include <stb_shared.h>
#include <avr/wdt.h>


class STB_BRAIN {
    private:
        KeywordsList Keywords;
        static const int settingsCount = 8;
        /* data */
    public:
        // by default everything is enabled
        int flags[cmdFlags::amountOfFlags] = {0};
        // arbritralily limited to 8 settings
        int settings[settingsCount][2] = {0};
        void receiveFlags(STB STB);
        void receiveSettings(STB STB);
        STB_BRAIN(/* args */);
        ~STB_BRAIN();
};
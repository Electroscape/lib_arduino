#pragma once

#include <stb_common.h>
#include <stb_shared.h>
#include <avr/wdt.h>

class STB_MOTHER
{
    private:
        /* data */
    public:
        STB_MOTHER(/* args */);
        ~STB_MOTHER();
        KeywordsList keyWords;

        void setFlag(STB STB, int brainNo, cmdFlags cmdFlag, bool status);
        void flagsCompleted(STB STB, int brainNo);

        void sendSetting(STB STB, int brainNo, settingCmds setting, int values[], int amountOfValues);
        void settingsCompleted(STB STB, int brainNo);
};



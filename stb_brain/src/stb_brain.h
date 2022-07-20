#pragma once

#include <stb_common.h>
#include <stb_shared.h>


class STB_BRAIN {
    private:
        KeywordsList Keywords;
        /* data */
    public:
        // by default everything is enabled
        int flags[cmdFlags::AmountOfFlags] = {0};
        bool receiveFlags(STB STB);
        STB_BRAIN(/* args */);
        ~STB_BRAIN();
};
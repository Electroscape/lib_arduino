#pragma once

#include <stb_common.h>
#include <stb_shared.h>

class STB_MOTHER
{
    private:
        /* data */
    public:
        STB_MOTHER(/* args */);
        ~STB_MOTHER();
        KeywordsList keyWords;

        bool setFlag(STB STB, int brainNo, cmdFlags cmdFlag, bool status);
};



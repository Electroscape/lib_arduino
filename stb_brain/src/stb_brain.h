#pragma once

#include <stb_shared.h>

class stb_brain
{
private:
    /* data */
public:
    flags flags();
    stb_brain(/* args */);
    ~stb_brain();
};

class flags {
    public:
    bool LED = true;
    bool RFID = true;
    bool OLED = true;
};

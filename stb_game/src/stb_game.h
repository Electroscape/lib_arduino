#pragma once
#include <avr/wdt.h>

class STB_GAME {
    private:
    int lastStage = -1;
    int stageIndex = 0;
    public:
    int stage = 0;
    // reset fnc?
    void setStageIndex();
    void stageUpdate();
};
#pragma once

#include <stb_mother.h>
#include <stb_keypadCmds.h>
#include <avr/wdt.h>

class STB_GAME {
    private:
    int lastStage = -1;
    int stageIndex = 0;
    public:
    int stageCount = 0;
    int stage = 0;
    STB_GAME(int stageCount);
    // reset fnc?
    void setStageIndex();
    void stageUpdate();
    void sendResult(STB_MOTHER &Mother, bool result, int brainNo=-1);
};
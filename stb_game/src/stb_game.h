#pragma once

#include <stb_mother.h>
#include <stb_keypadCmds.h>
#include <stb_oledCmds.h>
#include <avr/wdt.h>

class STB_GAME {
    private:
    int lastStage = -1;
    public:
    int stageIndex = 0;
    int stageCount = 0;
    int stage = 0;
    STB_GAME(int stageCount, int startingStage=1);
    // reset fnc?
    void setStageIndex();
    bool stageUpdate();
    static void sendResult(STB_MOTHER &Mother, bool result, int brainNo=-1);
    void setOledHeader(STB_MOTHER &Mother, char* msg, int brainNo=-1);
};
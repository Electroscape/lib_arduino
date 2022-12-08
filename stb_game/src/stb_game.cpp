#include "stb_game.h"


STB_GAME::STB_GAME(int stageCount) {
    stageCount = stageCount;
}

/**
 * @brief Set the Stage Index object
 * @todo safety considerations
*/
void STB_GAME::setStageIndex() {
    for (int i=0; i<stageCount; i++) {
        if (stage <= 1 << i) {
            stageIndex = i;
            Serial.print("stageIndex:");
            Serial.println(stageIndex);
            delay(1000);
            return;
        }
    }
    Serial.println(F("STAGEINDEX ERRROR!"));
    wdt_reset();
    delay(16000);
}


void STB_GAME::stageUpdate() {
    if (lastStage == stage) { return; }
    Serial.print("Stage is:");
    Serial.println(stage);
    setStageIndex();

    // check || stageIndex >= int(sizeof(stages))
    if (stageIndex < 0) {
        Serial.println(F("Stages out of index!"));
        delay(15000);
    }
    // important to do this before stageActions! otherwise we skip stages
    lastStage = stage;
}


/**
 * @brief sends keypad/rfid validity to be displayed on the oled
 * @param Mother 
 * @param result 
 * @param brainNo 
*/
void STB_GAME::sendResult(STB_MOTHER &Mother, bool result, int brainNo) {

    // prepare return msg with correct or incorrect
    char msg[10] = "";
    char noString[3] = "";
    strcpy(msg, keypadCmd.c_str());
    strcat(msg, KeywordsList::delimiter.c_str());

    if (result) {
        sprintf(noString, "%d", KeypadCmds::correct);
    } else {
        sprintf(noString, "%d", KeypadCmds::wrong);
    }

    strcat(msg, noString);
    Mother.sendCmdToSlave(msg, brainNo);
}

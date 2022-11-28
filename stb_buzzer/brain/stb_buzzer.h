#pragma once
#include "stb_buzzer_cmds.h"

class STB_BUZZER {
    void init(int pwmPin);
    void buzzerUpdate();
    void buzzerReset();
}
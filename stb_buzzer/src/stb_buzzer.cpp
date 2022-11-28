/**
 * @file stb_buzzer.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief  
 * @version 0.1
 * @date 2022-11-28
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include "stb_buzzer.h"


STB_BUZZER::STB_BUZZER() {};


/**
 * @brief initializes the buzzer
 * @param pwmPin 
*/
void STB_BUZZER::init(int pwmPin) {
    pinMode(pwmPin, OUTPUT);
    noTone(pwmPin);
    BuzzerPars.pin = pwmPin;
}


/**
 * @brief  resets the buzzer terminating the previous mode
*/
void STB_BUZZER::buzzerStop() {
    for (int i=0; i<BuzzerMaxStages; i++) {
        BuzzerPars.stage = -1;
        BuzzerPars.freq[i] = 0;
    }
    noTone(BuzzerPars.pin);
}


/**
 * @brief updates the buzzer inside the mainloop nonblocking
*/
void STB_BUZZER::update() {
    if (BuzzerPars.stage < 0) {return;}
    if (BuzzerPars.stage >= BuzzerMaxStages) {
        buzzerStop();
        return;
    }

    if (BuzzerPars.stage == 0 || millis() > BuzzerPars.timeStamp) {
        // moves next execution to after the on + offtime elapsed 
        BuzzerPars.timeStamp = millis() + BuzzerPars.onTime[BuzzerPars.stage] + BuzzerPars.offTime[BuzzerPars.stage];
        if (BuzzerPars.freq[BuzzerPars.stage] > 0) {
            tone(BuzzerPars.pin, BuzzerPars.freq[BuzzerPars.stage], BuzzerPars.onTime[BuzzerPars.stage]);
            BuzzerPars.stage++;
            return;
        } else {
            buzzerStop();
        }
    }
}



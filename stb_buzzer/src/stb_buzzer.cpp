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


/**
 * @brief initializes the buzzer
 * @param pwmPin 
*/
void STB_BUZZER::init(int pwmPin) {
    pinMode(pwmPin, OUTPUT);
    noTone(pwmPin);
    buzzerPin = pwmPin;
}


/**
 * @brief  resets the buzzer terminating the previous mode
*/
void STB_BUZZER::buzzerReset() {
    for (int i=0; i<BuzzerMaxStages; i++) {
        buzzerStage = -1;
        buzzerFreq[i] = 0;
    }
    noTone(buzzerPin);
}


/**
 * @brief updates the buzzer inside the mainloop nonblocking
*/
void STB_BUZZER::buzzerUpdate() {
    if (buzzerStage < 0) {return;}
    if (buzzerStage >= BuzzerMaxStages) {
        buzzerReset();
        return;
    }

    if (buzzerStage == 0 || millis() > buzzerTimeStamp) {
        // moves next execution to after the on + offtime elapsed 
        buzzerTimeStamp = millis() + buzzerOn[buzzerStage] + buzzerOff[buzzerStage];
        if (buzzerFreq[buzzerStage] > 0) {
            tone(buzzerPin, buzzerFreq[buzzerStage], buzzerOn[buzzerStage]);
            buzzerStage++;
            return;
        } else {
            buzzerReset();
        }
    }
}



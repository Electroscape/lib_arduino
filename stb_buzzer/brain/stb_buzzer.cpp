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
void init(int pwmPin) {
    pinMode(pwmPin, OUTPUT);
    noTone(pwmPin);
}


/**
 * @brief updates the buzzer inside the mainloop nonblocking
*/
void buzzerUpdate() {
    if (buzzerStage < 0) {return;}
    if (buzzerStage >= BuzzerMaxStages) {
        buzzerReset();
        return;
    }

    if (buzzerStage == 0 || millis() > buzzerTimeStamp) {
        // moves next execution to after the on + offtime elapsed 
        buzzerTimeStamp = millis() + buzzerOn[buzzerStage] + buzzerOff[buzzerStage];
        if (buzzerFreq[buzzerStage] > 0) {
            tone(BUZZER_PIN, buzzerFreq[buzzerStage], buzzerOn[buzzerStage]);
            buzzerStage++;
            return;
        } else {
            buzzerReset();
        }
    }
}


/**
 * @brief  resets the buzzer terminating the previous mode
*/
void buzzerReset() {
    for (int i=0; i<BuzzerMaxStages; i++) {
        buzzerStage = -1;
        buzzerFreq[i] = 0;
    }
}
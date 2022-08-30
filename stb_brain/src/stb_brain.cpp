/**
 * @file stb_brain.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "stb_brain.h"

STB_BRAIN::STB_BRAIN(): STB_(){
    STB_.begin();
    STB_.i2cScanner();
    STB_.defaultOled.setScrollMode(SCROLL_MODE_AUTO);
    for (int row=0; row<SETTINGS_CNT; row++) {
        settings[row][0] = -1;
    }
};
STB_BRAIN::~STB_BRAIN() {};


/**
 * @brief receives the flags send by the mother
 * @return true when flags have been received
 * Todo: make safety checks, cleanup FIX!
 */
void STB_BRAIN::receiveFlags() {

    STB_.dbgln("receiveflags");
    bool sendAck = false;

    while (true) {

        if (!rs485PollingCheck()) {continue;}

        while (STB_.rcvdPtr != NULL) {

            STB_.dbgln("STB_.rcvdPtr is: ");
            STB_.dbgln(STB_.rcvdPtr);

            if (strncmp(KeywordsList::flagKeyword.c_str(), STB_.rcvdPtr, KeywordsList::flagKeyword.length()) == 0) {
                
                STB_.rcvdPtr += strlen(KeywordsList::flagKeyword);
                STB_.dbgln(STB_.rcvdPtr);

                char line[16] = "";
                char* linePtr = strtok(line, "_"); 
                linePtr = strtok(NULL, "_"); 
                STB_.dbgln("Flagkeyword receivd");
                STB_.dbgln(linePtr);
            
                char noString[2];

                for (int keywordNo=0; keywordNo<cmdFlags::amountOfFlags; keywordNo++) { 
                    sprintf(noString, "%i", keywordNo);
                    if (strncmp(STB_.rcvdPtr, noString, 1) == 0) {
                        STB_.dbg("correct keyword for: ");
                        STB_.dbgln(String(keywordNo));
                        STB_.rcvdPtr += 2;
                        sprintf(noString, "%i", 1);
                        flags[keywordNo] = (strncmp(STB_.rcvdPtr, noString, 1) == 0);
                        sendAck = true;
                        break;
                    }
                }

            } else if (strncmp(KeywordsList::endFlagKeyword.c_str(), STB_.rcvdPtr, KeywordsList::endFlagKeyword.length()) == 0) {
                
                STB_.rs485SendAck();

                /*
                STB_.dbgln("all flags received");
                
                for (int keywordNo=0; keywordNo<cmdFlags::amountOfFlags; keywordNo++) {
                    if (flags[keywordNo] > 0) {
                        STB_.dbgln("enabled");
                    } else {
                        STB_.dbgln("disabled");
                    }
                }
                delay(300);
                */

                return;
            }

            if (sendAck) {
                STB_.rs485SendAck();
                sendAck = false;
            }

            STB_.rs485RcvdNextLn();
        }

        wdt_reset();

    }
    
    return;
}


/**
 * @brief receives Settings from the Mother
 * todo define how many settings can be saved, 
 * how many values each settings can have
 */
void STB_BRAIN::receiveSettings() {

    int f = 0;

    bool sendAck = false;
    char line[12] = "";
    char *linePtr;
    int row = 0, col = 0;
    STB_.dbgln("STB_BRAIN::receiveSettings");

    while (true) {

        if (!rs485PollingCheck()) {continue;}
        
        while (STB_.rcvdPtr != NULL) {
            
            if (strncmp(KeywordsList::endSettingKeyword.c_str(), STB_.rcvdPtr, strlen(KeywordsList::endSettingKeyword)) == 0) {  
                STB_.rs485SendAck();

                Serial.print("row is ");
                Serial.println(String(row));

                return;         
            }

            if (strncmp(KeywordsList::settingKeyword.c_str(), STB_.rcvdPtr, strlen(KeywordsList::settingKeyword)) == 0) {

                sendAck = true;
                // discard if the rows are used up, we dont want to write out of index
                if (row >= SETTINGS_CNT) {
                    STB_.dbgln("too many settings\nreceived");
                    continue;
                }

                // doesnt have a trailing "_" hence a +1 to get the value
                STB_.rcvdPtr += strlen(KeywordsList::settingKeyword) + 1;

                strcpy(line, STB_.rcvdPtr);
                linePtr = strtok(line, "_"); 
                col = 0;
                
                while (linePtr != NULL && col < SETTINGS_PARAMS) {
                    settings[row][col] = atoi(linePtr);
                    linePtr = strtok(NULL, "_");
                    col++;
                }
                row++;
            }

            if (sendAck) {
                STB_.rs485SendAck();
                sendAck = false;
            }

            STB_.rs485RcvdNextLn();
        }

    }

}


/**
 * @brief sets slaveNo and creates a pollstring to respond to
 * @param no 
 */
void STB_BRAIN::rs485SetSlaveAddr(int no) {
    slaveAddr = no;
    STB::dbgln(F("Slave responds to")); 
    char noString[2];
    sprintf(noString, "%d", no);
    strcpy(slavePollStr, KeywordsList::pollStr);
    strcat(slavePollStr, noString);
    Serial.println(slavePollStr);
    delay(2000);
}


/**
 * @brief checks if being polled and message is complete, msg stored in rcvd
 * @param message 
 * @return if slave is being polled and can send
 */
bool STB_BRAIN::rs485PollingCheck() {

    int index = 0;
    unsigned long startTime = millis();

    while ((millis() - startTime) < maxPollingWait) {

        if (Serial.available()) {

            if (slavePollStr[index] == Serial.read()) {
                index++;
                if (index > 5) {
                    if (STB_.rs485Receive()) {
                        // slave interpretation here alternatively resend request in else loop
                    }
                    delay(1);
                    return true;
                }
            } else {
                index = 0;
            }
        }
        
    }
    return false;
}


/**
 * @brief slave checks if being polled and responds with the buffer 
 * @return if bufferOut could be send 
 */
bool STB_BRAIN::rs485SlaveRespond() {

    if (!rs485PollingCheck()) {
        Serial.println(F("no buffer clearnce"));
        return false;
    }

    STB_.rs485SendBuffer();
    return true;
}


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

STB_BRAIN::STB_BRAIN() {
    for (int row=0; row<SETTINGS_CNT; row++) {
        settings[row][0] = -1;
    }
};
STB_BRAIN::~STB_BRAIN() {};


/**
 * @brief receives the flags send by the mother
 * @param STB 
 * @return true when flags have been received
 * Todo: make safety checks, cleanup FIX!
 */
void STB_BRAIN::receiveFlags(STB STB) {

    STB.dbgln("receiveflags");
    bool sendAck = false;

    while (true) {

        if (!STB.rs485PollingCheck()) {continue;}

        while (STB.rcvdPtr != NULL) {

            STB.dbgln("STB.rcvdPtr is: ");
            STB.dbgln(STB.rcvdPtr);

            if (strncmp((char *) Keywords.flagKeyword, STB.rcvdPtr, strlen(Keywords.flagKeyword)) == 0) {
                
                STB.rcvdPtr += strlen(Keywords.flagKeyword);
                STB.dbgln(STB.rcvdPtr);

                char line[16] = "";
                char* linePtr = strtok(line, "_"); 
                linePtr = strtok(NULL, "_"); 
                STB.dbgln("Flagkeyword receivd");
                STB.dbgln(linePtr);
            
                char noString[2];

                for (int keywordNo=0; keywordNo<cmdFlags::amountOfFlags; keywordNo++) { 
                    sprintf(noString, "%i", keywordNo);
                    if (strncmp(STB.rcvdPtr, noString, 1) == 0) {
                        STB.dbg("correct keyword for: ");
                        STB.dbgln(String(keywordNo));
                        STB.rcvdPtr += 2;
                        sprintf(noString, "%i", 1);
                        flags[keywordNo] = (strncmp(STB.rcvdPtr, noString, 1) == 0);
                        sendAck = true;
                        break;
                    }
                }

            } else if (strncmp((char *) Keywords.endFlagKeyword, STB.rcvdPtr, strlen(Keywords.endFlagKeyword)) == 0) {
                STB.rs485SendAck();
                STB.dbgln("all flags received");
                
                for (int keywordNo=0; keywordNo<cmdFlags::amountOfFlags; keywordNo++) {
                    if (flags[keywordNo] > 0) {
                        STB.dbgln("enabled");
                    } else {
                        STB.dbgln("disabled");
                    }
                }
                delay(2000);
                return;
            }

            if (sendAck) {
                STB.rs485SendAck();
                sendAck = false;
            }

            STB.rs485RcvdNextLn();
        }

        wdt_reset();

    }
    
    return;
}


/**
 * @brief receives Settings from the Mother
 * @param STB 
 * todo define how many settings can be saved, 
 * how many values each settings can have
 */
void STB_BRAIN::receiveSettings(STB STB) {

    bool sendAck = false;
    char line[12] = "";
    char *linePtr;
    int value;
    int row = 0, col = 0;

    while (true) {

        if (!STB.rs485PollingCheck()) {continue;}
        
        while (STB.rcvdPtr != NULL) {
            
            if (strncmp((char *) Keywords.endSettingKeyword, STB.rcvdPtr, strlen(Keywords.endSettingKeyword)) == 0) {  
                STB.rs485SendAck();
                return;         
            }

            if (strncmp((char *) Keywords.settingKeyword, STB.rcvdPtr, strlen(Keywords.settingKeyword)) == 0) {

                sendAck = true;
                // discard if the rows are used up, we dont want to write out of index
                if (row >= SETTINGS_CNT) {
                    STB.dbgln("too many settings\nreceived");
                    continue;
                }

                // doesnt have a trailing "_" hence a +1 to get the value
                STB.rcvdPtr += strlen(Keywords.settingKeyword) + 1;

                strcpy(line, STB.rcvdPtr);
                linePtr = strtok(line, "_"); 
                col = 0;
                while (linePtr != NULL && col < 3) {
                    value = atoi(linePtr);
                    STB.dbgln(String(value));
                    settings[row][col] = value;
                    STB.dbgln(String(settings[row][col]));
                    col++;
                    linePtr = strtok(NULL, "_");
                }
                row++;

            }

            if (sendAck) {
                STB.rs485SendAck();
                sendAck = false;
            }

            STB.rs485RcvdNextLn();
        }

    }
}
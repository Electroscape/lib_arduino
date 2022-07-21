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

STB_BRAIN::STB_BRAIN() {};
STB_BRAIN::~STB_BRAIN() {};


/**
 * @brief receives the flags send by the mother
 * @param STB 
 * @return true when flags have been received
 * Todo: make safety checks
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


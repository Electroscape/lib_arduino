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
 */
bool STB_BRAIN::receiveFlags(STB STB) {

    STB.dbgln("receiveflags");
    bool sendAck = false;

    while (true) {

        STB.rs485SlaveRespond();

        while (STB.rcvdPtr != NULL) {

            STB.dbg("STB.rcvdPtr is: ");
            STB.dbgln(STB.rcvdPtr);

            // use the char length to check this
            if (strncmp((char *) Keywords.flagKeyword, STB.rcvdPtr, strlen(Keywords.flagKeyword)) == 0) {
                
                char line[16] = "";
                char* linePtr = strtok(line, "_"); 
                linePtr = strtok(NULL, "_"); 
                STB.dbgln("Flagkeyword receivd");
                // add a safety check here
                STB.dbgln(linePtr);

                for (int keywordNo=0; keywordNo<cmdFlags::AmountOfFlags; keywordNo++) { 
                    if (keywordNo == (signed char) linePtr[strlen(Keywords.flagKeyword)]) {
                       STB.dbgln("correct keyword");
                       flags[keywordNo] = (int) STB.rcvdPtr[strlen(Keywords.flagKeyword) + 2];
                       sendAck = true;
                       break;
                    }
                }
            }
            STB.rs485RcvdNextLn();
        }

        if (sendAck) {
            STB.rs485SendAck();
            for (int keywordNo=0; keywordNo<cmdFlags::AmountOfFlags; keywordNo++) {
                if (flags[keywordNo] > 0) {
                    STB.dbgln("enabled");
                } else {
                    STB.dbgln("disabled");
                }
            }
            break;
        }
    }
    

    return true;
}


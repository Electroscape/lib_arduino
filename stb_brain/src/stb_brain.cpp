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
 * @brief adds given message to buffer
 * @param message 
 * @param containsCmd wether the message is send without control flow or send with a ack before being cleared from buffer
 */
void STB_BRAIN::addToBuffer(String message, bool containsCmd) {
    if (containsCmd) {
        outgoingCmd=true;
    }; 
    STB_.rs485AddToBuffer(message);
}


/**
 * @brief calls the STB startup fncs like STB.Begin and I2Cscanner
 */
void STB_BRAIN::begin() {
    STB_.begin();
    STB_.i2cScanner();
    STB_.defaultOled.setScrollMode(SCROLL_MODE_AUTO);
}


/**
 * @brief receives the flags send by the mother
 */
bool STB_BRAIN::receiveFlags() {

    if (strncmp(KeywordsList::flagKeyword.c_str(), STB_.rcvdPtr, KeywordsList::flagKeyword.length()) != 0) {
        return false;
    }     
    Serial.print("flags rcv: ");
    Serial.println(STB_.rcvdPtr);
    STB_.rcvdPtr += KeywordsList::flagKeyword.length();

    if (!STB_.rcvdPtr) { return false; };

    STB_.rs485SendAck();
    flags = atoi(STB_.rcvdPtr);
    Serial.print("flags are: ");
    Serial.println(flags);
    return true;
}


/**
 * @brief receives Settings from the Mother
 * todo define how many settings can be saved, 
 * how many values each settings can have
 */
bool STB_BRAIN::receiveSetting() {

    /*

    bool sendAck = false;
    char line[12] = "";
    char *linePtr;
    int row = 0, col = 0;

    if (strncmp(KeywordsList::settingKeyword.c_str(), STB_.rcvdPtr, KeywordsList::settingKeyword.length()) != 0) {
        return;
    }

    // discard if the rows are used up, we dont want to write out of index
    if (row >= SETTINGS_CNT) {
        STB_.dbgln(F("overflow!"));
        continue;
    }

        // doesnt have a trailing "_" hence a +1 to get the value
        STB_.rcvdPtr += KeywordsList::settingKeyword.length() + 1;

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
    */
    return true;
}


/**
 * @brief sets slaveNo and creates a pollstring to respond to
 * @param no 
 */
void STB_BRAIN::setSlaveAddr(int no) {
    slaveAddr = no;
    STB_.dbgln(F("Slave responds to")); 
    char noString[2];
    sprintf(noString, "%d", no);
    strcpy(slavePollStr, KeywordsList::pollStr.c_str());
    strcat(slavePollStr, noString);
    strcpy(slavePushStr, KeywordsList::pushStr.c_str());
    strcat(slavePushStr, noString);
    Serial.println(slavePollStr);
    Serial.println(slavePushStr);
    delay(2000);
}


/**
 * @brief checks if being polled and message is complete, msg stored in rcvd
 * @param message 
 * @return -1 if slave didnt get any message 0 for incoming cmd and 1 for being polled
 * TODO: may need to check how bool handles -1 0 and 1
 */
int STB_BRAIN::pollingCheck() {

    int indexPoll = 0;
    int indexPush = 0;
    unsigned long startTime = millis();

    char read;

    while ((millis() - startTime) < STB_.maxPollingWait) {

        if (Serial.available()) {
            
            read = Serial.read();

            if (slavePollStr[indexPoll] == read) {
                indexPoll++;
                if (indexPoll == 5) {
                    delay(1);
                    return 1;
                }
            } else {
                indexPoll = 0;
            }

            if (slavePushStr[indexPush] == read) {
                indexPush++;
                if (indexPush == 5) {
                    STB_.rs485Receive();
                    delay(1);
                    return 0;
                } 
            } else {
                indexPush = 0;
            }

        }
        
    }
    return -1;
}


/**
 * @brief slave checks if being polled and responds with the buffer 
 * @return true if slave is being pushed and needs to evaluate
 */
bool STB_BRAIN::slaveRespond() {

    delay(1);
    int res = pollingCheck();
    delay(1);

    if ( res == 0 ) {
        return true;
    }

    if (res == 1 && STB_.rs485SendBuffer(outgoingCmd)) {
        // Serial.println(F("cmd send clearing"));
        // STB_.clearBuffer();
        outgoingCmd = false;
    }

    return false;
}


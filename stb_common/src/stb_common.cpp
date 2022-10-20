/**
 * @file stb_common.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "stb_common.h"


/**
 * @brief Construct a new stb::stb dbg object, creates and oled instance to be used a debug print
 */
STB::STB() {}


/**
 * @brief starts serial and prints out information about the program to be run
 */
void STB::begin() {
    serialInit();
    STB_OLED::oledInit(&defaultOled , SH1106_128x64);
    printInfo();
}


/**
 *  Initialise I2C, Serial and RS485 busses
 * 
 *  @return true (bool) true on success
 *  @param void void
 */
bool STB::serialInit() {
    Wire.begin();
    Wire.setClock(i2cClkSpeed);
    Serial.begin(57600); // 115200 might be too high
    Serial.setTimeout(rs485timeout);
    delay(100);
    pinMode(MAX_CTRL_PIN, OUTPUT);
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
    return true;
}


void STB::printInfo() {

    defaultOled.println(F("\n\n+--------------------+"));
    defaultOled.println(F("|    Electroscape    |"));
    defaultOled.println(F("+--------------------+"));

    /*
    printWithHeader("!header_begin");
    #ifdef title
        printWithHeader(title);
    #endif
    #ifdef version
        printWithHeader("v" + version);
    #endif
    #ifdef versiondate
        printWithHeader(versionDate);
    #endif
    printWithHeader("!header_end");
    printWithHeader("!setup_begin");
    */

    delay(300);
}


/**
 * @brief prints a setup end to serial
 */
void STB::printSetupEnd() {
    // printWithHeader("!setup_end");
    // Serial.println();
    /// Serial.flush(); 
    dbgln(F("\n===START===\n"));
}


/**
 * @brief Prints a message on serial and OLED and potentially interface pi, latter is TBD
 * @param message 
 */
void STB::dbg(String message) {
    defaultOled.print(message);
    Serial.print(message);
    Serial.flush();
}


/**
 * @brief Prints a message on serial and OLED and potentially interface pi, latter is TBD
 * @param message 
 */
void STB::dbgln(String message) {
    defaultOled.println(message);
    Serial.println(message);
    Serial.flush();
}


/**
 * @brief  clears the outgoing buffer
 * 
*/
void STB::clearBuffer() {
    bufferOut[0] = '\0';
}


/**
 * @brief add the content with Newline in the end to the outgoing buffer
 * @param message
 * @return if place was available in the bufferOut
 */
bool STB::rs485AddToBuffer(String message) {
    if (strlen(bufferOut) + message.length() + 2 <= bufferSize) {
        strcat(bufferOut, message.c_str());
        strcat(bufferOut, "\n");
        return true;
    }
    return false;
}


/**
 * @brief  simply writes bufferout, persistent determine if its cleared or not after the msg
 * @param persistent 
*/
void STB::rs485Write(bool persistent) {
    digitalWrite(MAX_CTRL_PIN, MAX485_WRITE);
    delayMicroseconds(writedelay);
    Serial.print(bufferOut);
    Serial.println(KeywordsList::eof);
    // extra newline for readaility when monitoring, to tell different senders apart
    Serial.println();
    Serial.flush();
    delayMicroseconds(writedelay);
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
    if (!persistent) {
        clearBuffer();
    }
}


/**
 * @brief received rs485 to rcvd buffer, also resets the buffer before writing to it
 * TODO: resend request if EOF is not found
 */
bool STB::rs485Receive() {

    memset(rcvd, 0, bufferSize);
    int bufferpos = 0;
    int eofIndex = 0;
    unsigned long slotStart = millis();
    
    while ((millis() - slotStart) < maxResponseTime && bufferpos < bufferSize) {

        if (Serial.available()) {
            
            rcvd[bufferpos] = Serial.read();

            /*
            rcvdPtr = rcvd;
            if (memcmp(rcvdPtr[bufferpos-strlen(eof)] ) == 0) {

            }
            */
           
            if (rcvd[bufferpos] == KeywordsList::eof[eofIndex]) {
                eofIndex++;
                if (eofIndex == 4) { 
                    rcvd[bufferpos+1] = '\0';
                    rcvdPtr = &rcvd[0];
                    bufferSplit = false;
                    return true;
                }
            } else {
                eofIndex = 0;
            }
            bufferpos++;
        }
    }

    return false;
}

/**
 * @brief  
 * @return int -1 if no ack, if no ackNR found-> 0, otherwise AckNr
*/
int STB::checkAck() {
    if (memcmp(KeywordsList::ACK.c_str(), rcvdPtr, KeywordsList::ACK.length()) != 0) { return -1; }
    int no;
    if (sscanf(Brain.STB_.rcvdPtr,"%d", no) <= 0) {
        return 0;
    }
    return no;
}


/**
 * @brief      // only used in setup, to prevent a desync between mother and brain
 * @param ackNr 
*/
void STB::negotiateAck(int ackNr) {
    int ackNr = -1;
    while (ackNr < 2) {
        rs485Receive();
        if (ackNr < 2) {
            ackNr = checkAck();
        } else {
            checkNack();
        }

    }

}


bool STB::checkNack() {
    return (memcmp(KeywordsList::NACK.c_str(), rcvdPtr, KeywordsList::NACK.length()) != 0);
}


/**
 * @brief sends the acknowledge msg
 */
void STB::rs485SendAck(int ackNr=1) {
    char msg[8];
    char noString[3] = "";
    strcpy(msg, KeywordsList::ACK);
    strcat(msg, KeywordsList::delimiter);
    sprintf(noString, "%i", ackNr);
    strcat(msg, noString);
    rs485AddToBuffer(msg);
    rs485Write();
}


/**
 * @brief send buffer and waits for ack response if flag was set
 * @return if ack was received, if not a cmd returns true
 */
bool STB::rs485SendBuffer(bool isCmd) {
    rs485Write(isCmd);
    if (!isCmd) {return true;}
    rs485Receive();
    while (true) {
        Serial.println(rcvdPtr);
        int ackNr = checkAck();
        if (ackNr > 0) { 
            clearBuffer();
            return true; 
        }
        if (!rs485RcvdNextLn()) {return false;}
    }
    
}


/**
 * @brief return the next line in the rcvd buffer
 * @param line 
 * @return if rcvd buffer is empty, TODO: make sure this also picks up empty lines
 */
bool STB::rs485RcvdNextLn() {
    if (bufferSplit) {
        rcvdPtr = strtok(NULL, "\n");
    } else {
        rcvdPtr = strtok(rcvd, "\n");
        bufferSplit = true;
    }
    return (rcvdPtr != NULL);
}


/**
 *  @brief Prints out what I2C addresses responded on the bus
 *  @return void
 *  @param void void
 */
bool STB::i2cScanner() {
    Serial.println();
    defaultOled.clear();
    String hexAddr = "";
    for (byte i = 8; i < 120; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {
            dbg(F("0x"));
            hexAddr = String(i, HEX);
            hexAddr.toUpperCase();
            dbg(hexAddr);
            dbg(F(" "));
            printI2cDeviceName(i);
            delay(1);  
        }              
    }                  
    delay(3000);
    return true;
}


/**
 * @brief mapping of I2C address
 * 
 * @param deviceNo 
 * @return char 
 */
void STB::printI2cDeviceName (int deviceNo) {
    switch (deviceNo) {
        case 56: dbgln(F("Keypad (default)")); break;  //  0x38
        case 57: dbgln(F("Keypad/IO")); break;         //  0x3A
        case 58: dbgln(F("Keypad/IO")); break;         //  0x39
        case 59: dbgln(F("Keypad/IO")); break;         //  0x3B
        case 60: dbgln(F("Oled  (default)")); break;   //  0x3C
        case 61: dbgln(F("Oled")); break;              //  0x3D
        case 63: dbgln(F("Relay")); break;             //  0x3F
        default: dbgln(F("Unknown")); break;
    }
}


/**
 * @brief restarts the arduino
 */
void STB::softwareReset() {
    Serial.println(F("Restarting in"));
    delay(50);
    for (byte i = 3; i>0; i--) {
        Serial.println(i);
        delay(100);
    }
    asm volatile ("  jmp 0");
}





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
    Serial.begin(115200);
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


#ifdef True
/**
 * legacy at this point, also does not handle bus clearance
 * 
 *  @return void
 *  @param message (String) message to be printed
 *  @param source (String) soure of the message default is "SYS"
 */
void STB::printWithHeader(String message, String source) {
    Serial.flush();
    digitalWrite(MAX_CTRL_PIN, MAX485_WRITE);
    Serial.println();
    Serial.print("!Br,");
    Serial.print(source);
    Serial.print(",");
    Serial.print(message);
    Serial.println(",Done.");
    Serial.flush();
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
}
#endif


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
 * @brief 
 * todo check if message gets too long including EOF
 * @param message 
 * @return if message was written or bus clearance didnt occur
 */
void STB::rs485Write() {

    digitalWrite(MAX_CTRL_PIN, MAX485_WRITE);
    Serial.print(bufferOut);
    Serial.println(KeywordsList::eof);
    Serial.println();
    Serial.flush();
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
    memset(bufferOut, 0, bufferSize);

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
                    rcvdPtr = strtok(rcvd, "\n");
                    dbgln(rcvdPtr);
                    // possible problem here 
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
 * @brief sends the acknowledge msg
 */
void STB::rs485SendAck() {
    rs485AddToBuffer(KeywordsList::ACK);
    rs485Write();
}


/**
 * @brief send buffer and waits for ack response if flag was set
 * @return if ack was received, if not a cmd returns true
 */
bool STB::rs485SendBuffer(bool isCmd) {
    rs485Write();
    if (!isCmd) {return true;}
    rs485Receive();
    while (rs485RcvdNextLn()) {
        dbgln(rcvdPtr);
        if (memcmp(KeywordsList::ACK.c_str(), rcvdPtr, KeywordsList::ACK.length()) == 0) { 
            dbgln("Ack rcvd");
            return true; 
        }
    }
    return false;
}


/**
 * @brief return the next line in the rcvd buffer
 * @param line 
 * @return if rcvd buffer is empty
 */
bool STB::rs485RcvdNextLn() {
    rcvdPtr = strtok(NULL, "\n");
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





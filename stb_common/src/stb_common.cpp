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

    delay(300);
}

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


/**
 * @brief prints a setup end to serial
 */
void STB::printSetupEnd() {
    printWithHeader("!setup_end");
    Serial.println();
    Serial.flush(); 
    dbgln("\n===START===\n");
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


int STB::rs485getPolledSlave() {
    return polledSlave;
}


int STB::rs485getSlaveCnt() {
    return slaveCount;
}


/**
 * @brief sets Master along with relay initialisation
 */
void STB::rs485SetToMaster() {
    // TODO: may need to add a parameter for pins and initvalues
    defaultOled.setScrollMode(SCROLL_MODE_AUTO);
    isMaster = true;
    int relayPins[8] = {0,1,2,3,4,5,6,7};
    int relayInitVals[8] = {1,1,1,1,1,1,1,1};
    relayInit(motherRelay, relayPins, relayInitVals);
}


/**
 * @brief sets slaveNo and creates a pollstring to respond to
 * @param no 
 */
void STB::rs485SetSlaveAddr(int no) {
    slaveAddr = no;
    dbgln("Slave responds to"); 
    char noString[2];
    sprintf(noString, "%d", no);
    strcpy(slavePollStr, pollStr);
    strcat(slavePollStr, noString);
    Serial.println(slavePollStr);
    delay(2000);
}


/**
 * @brief defines how many slaves are being polled
 * @param count 
 */
void STB::rs485SetSlaveCount(int count) {
    slaveCount = count;
}


/**
 * @brief polls the bus slaves and forwards the input to cmdInterpreter
 */
void STB::rs485PerformPoll() {
    polledSlave++;
    if (polledSlave >= slaveCount) {
        polledSlave = 0;
    }

    rs485setSlaveAsTgt(polledSlave);
    // message.concat("line1\n2\n3\nlastline");
    rs485Write();
    rs485Receive();

    if (strlen(rcvd) > 0) {
        // TODO: new input receive here
        dbgln(rcvd);
    }
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
 * @brief slave checks if being polled and responds with the buffer 
 * @return if bufferOut could be send 
 */
bool STB::rs485SlaveRespond() {

    if (!rs485PollingCheck()) {
        Serial.println("no buffer clearnce");
        return false;
    }

    rs485Write();
    return true;
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
    Serial.println(eof);
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
           
            if (rcvd[bufferpos] == eof[eofIndex]) {
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
 * @brief WIP adds string of slave to be polled into the buffer
 * @param slaveNo 
 */
void STB::rs485setSlaveAsTgt(int slaveNo) {
    // there should not be other data left here anyways, alternativle use strCat
    strcpy(bufferOut, pollStr);
    char slaveNoStr[3];
    sprintf(slaveNoStr, "%i", slaveNo);
    strcat(bufferOut, slaveNoStr);
    strcat(bufferOut, "\n");
    // this will put the slaveStr in beginning of the buffer
};


/**
 * @brief sends the acknowledge msg
 */
void STB::rs485SendAck() {
    rs485AddToBuffer(ACK);
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
        if (memcmp(ACK, rcvdPtr, strlen(ACK)) == 0) { 
            dbgln("Ack rcvd");
            return true; 
        }
    }
    return false;
}


/**
 * @brief checks if being polled and message is complete, msg stored in rcvd
 * @param message 
 * @return if slave is being polled and can send
 */
bool STB::rs485PollingCheck() {

    int index = 0;
    unsigned long startTime = millis();

    while ((millis() - startTime) < maxPollingWait) {

        if (Serial.available()) {

            if (slavePollStr[index] == Serial.read()) {
                index++;
                if (index > 5) {
                    if (rs485Receive()) {
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
 * @brief return the next line in the rcvd buffer
 * @param line 
 * @return if rcvd buffer is empty
 */
bool STB::rs485RcvdNextLn() {
    rcvdPtr = strtok(NULL, "\n");
    return (rcvdPtr != NULL);
}


/**
 * @brief send the given message to the designated slave
 * @param slaveNo 
 * @param message 
 * @return if message got acknowled
 */
bool STB::rs485SendCmdToSlave(int slaveNo, char* message) {
    rs485setSlaveAsTgt(slaveNo);
    // newline is handled by teh aboive so nothing to worry
    strcat(bufferOut, message);
    dbgln("cmd to slave");
    dbgln(bufferOut);
    rs485SendBuffer();
    return true;
};


/**
 *  @brief Prints out what I2C addresses responded on the bus
 *  @return void
 *  @param void void
 */
bool STB::i2cScanner() {
    Serial.println();
    dbgln("   I2C Scanner:");
    String hexAddr = "";
    for (byte i = 8; i < 120; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {
            dbg("Found: ");
            dbg(" (0x");
            hexAddr = String(i, HEX);
            hexAddr.toUpperCase();
            dbg(hexAddr);
            dbgln(")  ");
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
        case 56: dbgln("Keypad (default)"); break;  //  0x38
        case 58: dbgln("Keypad/IO"); break;         //  0x39
        case 57: dbgln("Keypad/IO"); break;         //  0x3A
        case 59: dbgln("Keypad/IO"); break;         //  0x3B
        case 60: dbgln("Oled  (default)"); break;   //  0x3C
        case 61: dbgln("Oled"); break;              //  0x3D
        case 63: dbgln("Relay"); break;             //  0x3F
        default: dbgln("Unknown"); break;
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


/**
 * @brief initializes the given relay along with init states
 * @param relay (PCF8574) relay instance
 * @param pins (int) pin numbers
 * @param initvals (int) init value
 * @param amount (int) amount of relays to be initialized
 * @return bool
 */
bool STB::relayInit(PCF8574 &relay, int pins[], int initvals[], int amount) {
    String relayString = String(RELAY_I2C_ADD, HEX);
    relayString.toUpperCase();
    dbgln("relayinit on " + relayString); 

    relay.begin(RELAY_I2C_ADD);
    
    for (int i = 0; i < amount; i++) {
        relay.pinMode(pins[i], OUTPUT);
        relay.digitalWrite(pins[i], initvals[i]);
        dbg("Relay ["); dbg(String(pins[i])); dbg("] set to "); dbgln(String(initvals[i]));
    }
    delay(500);
    return true;
}





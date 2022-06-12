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
    printInfo();
    STB_OLED::oledInit(&defaultOled , SH1106_128x64);
    defaultOled.setFont(Adafruit5x7);
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
    Serial.println(F("+--------------------+"));
    Serial.println(F("|    Electroscape    |"));
    Serial.println(F("+--------------------+"));
    Serial.flush();
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

/**
 * @brief sets Master along with relay initialisation
 */
void STB::rs485SetToMaster() {
    // TODO: may need to add a parameter for pins and initvalues
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
    dbgln("Slave respons to"); 
    slavePollStr = "!Poll";
    slavePollStr.concat(no);
    dbgln(slavePollStr);
    delay(2000);
}

/**
 * @brief polls the bus slaves and forwards the input to cmdInterpreter
 */
void STB::rs485PerformPoll() {
    String message = "";

    for (int slaveNo = 0; slaveNo < slaveCount; slaveNo++) {

        message = "!Poll";
        message.concat(slaveNo);
        message.concat("\n");
        // message.concat("line1\n2\n3\nlastline");
        rs485Write(message);
        rs485Receive();

        if (strlen(rcvd) > 0) {

            // TODO: new input receive here
            // defaultOled.println(rcvd);
        }
    }
}


/**
 * @brief
 *
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
 * 
 * @return if bufferOut could be send 
 */
bool STB::rs485SlaveRespond() {

    if (!rs485PollingCheck()) {
        dbgln("no buffer clearnce");
        return false;
    }



    rs485Write(bufferOut);
    memset(bufferOut, 0, bufferSize);
    return true;
}


/**
 * @brief 
 * 
 * @param message 
 * @return if message was written or bus clearance didnt occur
 */
void STB::rs485Write(String message) {

    digitalWrite(MAX_CTRL_PIN, MAX485_WRITE);
    Serial.println(message);
    Serial.println(eof);
    Serial.flush();
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);

    if (!isMaster) {
        dbgln("RS485 out: \n" + message);
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
    long slotStart = millis();
    
    while ((millis() - slotStart) < maxResponseTime && bufferpos < bufferSize) {

        if (Serial.available()) {
            
            rcvd[bufferpos] = Serial.read();

            if (rcvd[bufferpos++] == eof[eofIndex++]) {
                if (eofIndex == 4) { 
                    rcvdPtr = strtok(rcvd, "\n"); 
                    return true;
                }
            } else {
                eofIndex = 0;
            }
        }
    }
    return false;
}



/**
 * @brief 
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
bool STB::rs485RcvdNextLn(char* line) {
    if (rcvdPtr != NULL) {
        strcpy(line, rcvdPtr);
        rcvdPtr = strtok(NULL, "\n");
        return true;
    }
    return false;
}




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
    dbgln("I2C scan complete");
    return true;
}

/**
 * @brief 
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
 * 
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
 * @param relay (PCF8574) relay instance
 * @param pins (int) pin numbers
 * @param initvals (int) init value
 * @param amount (int) amount of relays to be initialized
 * @return bool
 */
bool STB::relayInit(PCF8574 &relay, int pins[], int initvals[], int amount=8) {
    String relayString = String(RELAY_I2C_ADD, HEX);
    relayString.toUpperCase();
    dbgln("relayinit on " + relayString); 

    relay.begin(RELAY_I2C_ADD);
    
    for (int i = 0; i < amount; i++) {
        relay.pinMode(pins[i], OUTPUT);
        relay.digitalWrite(pins[i], initvals[i]);
        dbg("Relay ["); dbg(String(pins[i])); dbg("] set to "); dbgln(String(initvals[i]));
    }
    dbgln(F("\nrelay init successful"));
    return true;
}





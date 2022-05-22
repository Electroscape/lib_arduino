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
 * 
 */
STB::STB() {}


/**
 * @brief starts serial and prints out information about the program to be run
 * 
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
    delay(20);
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
 *  Activates MAX485 to write on the rs485 bus. Serial print the message with 
 *  the known frame to the frontend
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
 * 
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

void STB::rs485SetToMaster() {
    isMaster = true;
    // quick and dirty, do not leave this past testing
    int relayPins[8] = {0,1,2,3,4,5,6,7};
    int relayInitVals[8] = {1,1,1,1,1,1,1,1};
    relayInit(motherRelay, relayPins, relayInitVals);
}

void STB::rs485SetSlaveAddr(int no) {
    slaveAddr = no;
    dbgln("Slave respons to"); 
    slavePollStr = "!Poll";
    slavePollStr.concat(no);
    dbgln(slavePollStr);
    delay(2000);
}

void STB::rs485PerformPoll() {
    String message = "";
    char rcvd[buffersize] = "";
    int bufferpos, eofIndex;

    for (int slaveNo = 0; slaveNo < slaveCount; slaveNo++) {
        bufferpos = 0;
        eofIndex = 0;
        message = "!Poll";
        message.concat(slaveNo);
        long slotStart = millis();
        rs485Write(message);

        /*
        // use this to read what teh mother recieves 
        while () {
            if (Serial.available()) {
                Serial.write(Serial.read());
            }
        }
        */
        
        while ((millis() - slotStart) < maxResponseTime && bufferpos < buffersize) {
            
            if (Serial.available()) {
                
                rcvd[bufferpos] = Serial.read();

                if (rcvd[bufferpos] == eof[eofIndex]) {
                    eofIndex++;
                    if (eofIndex == 4) { 
                        // only interpret valid frame
                        cmdInterpreter(rcvd, slaveNo);
                        break; 
                    }
                } else {
                    // eofIndex = 0;
                }

                bufferpos++;
            }
        }

        if (bufferpos > 0) {
            defaultOled.println(rcvd);
        }
    }
}


bool STB::rs485Write(String message) {

    // failed to get a bus clearance, adding msg to buffer
    if (!isMaster && !rs485PollingCheck()) {
        // dbgln("not cleared to\n write RS485");
        // Todo: create and add to buffer
        // maybe also a fnc to be called inside the loop
        return false;
    }

    digitalWrite(MAX_CTRL_PIN, MAX485_WRITE);
    Serial.println(message);
    if (!isMaster) {Serial.println(eof);}
    Serial.flush();
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);

    if (!isMaster) {
        dbgln("RS485 out: \n" + message);
    }

    return true;
}



/**
 * @brief 
 * @param message 
 * @return if slave is being polled and can send
 */
bool STB::rs485PollingCheck() {

    char rcvd;
    int index = 0;
    unsigned long startTime = millis();

    while ((millis() - startTime) < maxPollingWait) {
        if (Serial.available()) {
            if (slavePollStr[index] == Serial.read()) {
                index++;
                if (index > 5) {
                    // small delay needed otherwise brain and mother collide on other brains
                    delay(1);
                    return true;
                }
            } else {
                index == 0;
            }
        }
    }

    return false;
}

bool STB::rs485SendRelayCmd(int relayNo, int value) {
    String msg = relayKeyword;
    msg.concat("_");
    msg.concat(relayNo);
    msg.concat("_");
    msg.concat(value);
    // till fixed
    msg.concat("_");
    return (rs485Write(msg));
}


void STB::cmdInterpreter(char *rcvd, int slaveNo) {
    defaultOled.print("rcvd cmd from "); defaultOled.println(slaveNo);       
    defaultOled.println(rcvd);        
    defaultOled.println(relayKeyword);        

    if (strncmp(rcvd, relayKeyword, 6) == 0) {
        defaultOled.println("Keyword Relay!");    
        char* splits = strtok(rcvd, delimiter);
        // already go to the 2nd entry
        splits = strtok(NULL, delimiter);

        int i = 0;
        int values[2] = {0,0};
        while (splits && i < 2) {
            // ASCII code, the numbers (digits) start from 48
            values[i++] = (int) *splits - 48;
            splits = strtok(NULL, delimiter);
        }
        if (i==2) {
            defaultOled.print("values are: "); 
            defaultOled.print(values[0]);
            defaultOled.print("  ");
            defaultOled.println(values[1]);
            motherRelay.digitalWrite(values[0], values[1]);
        }
    }  
    delay(2000);
}

/**
 *  Prints out what I2C addresses respond on the bus
 * 
 *  @return void
 *  @param void void
 */
bool STB::i2cScanner() {
    Serial.println();
    dbgln("   I2C Scanner:");
    delay(10);
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
    delay(10);
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





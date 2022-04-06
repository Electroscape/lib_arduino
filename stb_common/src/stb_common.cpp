#include "stb_common.h"
#include <Wire.h>
/*
STB::Brain::Brain(String BrainName) {
}

STB::Brain::~Brain() {
}
*/

void STB::begin() {
    serialInit();
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
    delay(100);
    pinMode(MAX_CTRL_PIN, OUTPUT);
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
        printWithHeader(version);
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
    digitalWrite(MAX_CTRL_PIN, MAX485_WRITE);
    Serial.println();
    Serial.print("!Br,");
    Serial.print(source);
    Serial.print(",");
    Serial.print(message);
    Serial.println(",Done.");
    // check if this can be reduced 
    delay(50);
    digitalWrite(MAX_CTRL_PIN, MAX485_READ);
}

void STB::printSetupEnd() {
    printWithHeader("!setup_end");
    Serial.println(); Serial.println("===================START====================="); Serial.println();
}


/*
void stb_namespace::heartbeat() {
    if (millis() - lastHeartbeat >= heartbeatFrequency) {
        printWithHeader(F("Hearthbeat"), F("SYS"));
    }
}
*/

/**
 *  assembly command makes the mcontroller jumps to memory location 0 
 * 
 *  @return void
 *  @param void void
 */

/**
 *  Prints out what I2C addresses respond on the bus
 * 
 *  @return void
 *  @param void void
 */
bool STB::i2cScanner() {
    /*
    map<int, char> addressMap;
    addressMap[63] = "Relay";
    addressMap[56] = "Keypad (default)";
    addressMap[57] = "Keypad";
    addressMap[58] = "Keypad";
    addressMap[59] = "Keypad";
    addressMap[60] = "Oled  (default)";
    addressMap[61] = "Oled";
    */

    Serial.println();
    Serial.println(F("I2C scanner:"));
    Serial.println(F("Scanning..."));
    byte count = 0;
    for (byte i = 8; i < 120; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {
            Serial.print("Found address: ");
            // Serial.print(i, DEC);
            Serial.print(" (0x");
            Serial.print(i, HEX);
            Serial.print(")  ");
            printI2cDeviceName(i);
            count++;
            delay(1);  
        }              
    }                  
    Serial.println("Done.");
    Serial.print("Found ");
    Serial.print(count, DEC);
    Serial.println(" device(s).");

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
        case 63: Serial.println("Relay"); break;
        case 56: Serial.println("Keypad (default)"); break;
        case 57: Serial.println("Keypad/IO"); break;
        case 58: Serial.println("Keypad/IO"); break;
        case 59: Serial.println("Keypad/IO"); break;
        case 60: Serial.println("Oled  (default)"); break;
        case 61: Serial.println("Oled"); break;
        default: Serial.println("Unknown"); break;
    }
}

/**
 * @brief restarts teh arduino
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

    Serial.print(F("\n relay init on address ")); Serial.println(RELAY_I2C_ADD);
    relay.begin(RELAY_I2C_ADD);
    
    for (int i = 0; i < amount; i++) {
        relay.pinMode(pins[i], OUTPUT);
        relay.digitalWrite(pins[i], initvals[i]);
        Serial.print("     ");
        Serial.print("Relay ["); Serial.print(pins[i]); Serial.print("] set to "); Serial.println(initvals[i]);
    }
    Serial.print(F(" successful"));
    return true;
}


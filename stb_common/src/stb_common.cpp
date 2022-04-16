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
STB::STB() {
    begin();
    STB_OLED::oledInit(&defaultOled , SH1106_128x64);
}


/**
 * @brief starts serial and prints out information about the program to be run
 * 
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


/**
 * @brief prints a setup end to serial
 * 
 */
void STB::printSetupEnd() {
    printWithHeader("!setup_end");
    Serial.println(); Serial.println("===================START====================="); Serial.println();
}

/**
 * @brief Prints a message on serial and OLED and potentially interface pi, latter is TBD
 * @param message 
 */
void STB::dbg(String message) {
    STB::defaultOled.print(message);
    Serial.print(message);
}

/**
 * @brief Prints a message on serial and OLED and potentially interface pi, latter is TBD
 * @param message 
 */
void STB::dbgln(String message) {
    STB::defaultOled.println(message);
    Serial.println(message);
}

/**
 *  Prints out what I2C addresses respond on the bus
 * 
 *  @return void
 *  @param void void
 */
bool STB::i2cScanner() {
    Serial.println(F("\n\nI2C scanner:"));
    Serial.println(F("Scanning..."));
    delay(10);
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
        case 56: Serial.println("Keypad (default)"); break;
        case 57: Serial.println("Keypad/IO"); break;
        case 58: Serial.println("Keypad/IO"); break;
        case 59: Serial.println("Keypad/IO"); break;
        case 60: Serial.println("Oled  (default)"); break;
        case 61: Serial.println("Oled"); break;
        case 63: Serial.println("Relay"); break;
        default: Serial.println("Unknown"); break;
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

    Serial.print(F("\nrelay init on address ")); Serial.print(RELAY_I2C_ADD); Serial.print(F(" ...\n"));
    relay.begin(RELAY_I2C_ADD);
    
    for (int i = 0; i < amount; i++) {
        relay.pinMode(pins[i], OUTPUT);
        relay.digitalWrite(pins[i], initvals[i]);
        Serial.print("     ");
        Serial.print("Relay ["); Serial.print(pins[i]); Serial.print("] set to "); Serial.println(initvals[i]);
    }
    Serial.println(F("\nrelay init successful"));
    return true;
}





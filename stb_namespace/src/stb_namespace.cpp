#include "stb_namespace.h"
#include <PCF8574.h>

/*
STB::Brain::Brain(String BrainName) {
}

STB::Brain::~Brain() {
}
*/

void STB::begin() {
    serial_init();
    print_info();
}

/**
 *  Initialise I2C, Serial and RS485 busses
 * 
 *  @return true (bool) true on success
 *  @param void void
 */
bool STB::serial_init() {
    Wire.begin();
    Serial.begin(115200);
    delay(100);
    pinMode(MAX_CTRL_PIN, OUTPUT);
    return true;
}

void STB::print_info() {

    Serial.println(F("+-----------------------------------+"));
    Serial.println(F("|    TeamEscape HH&S ENGINEERING    |"));
    Serial.println(F("+-----------------------------------+"));
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

void STB::print_setup_end() {
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
/*
void STB::softwareReset() {
    Serial.println(F("Restarting in"));
    delay(50);
    for (byte i = 3; i > 0; i--) {
        Serial.println(i);
        delay(100);
    }
    asm volatile("  jmp 0");
}
*/

/**
 *  Scans the I2C bus all 
 * 
 *  @return void
 *  @param void void
 */
/*
bool STB::i2cScanner() {
    Serial.println();
    Serial.println(F("I2C scanner:"));
    Serial.println(F("Scanning..."));
    byte count = 0;
    for (byte i = 8; i < 120; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {
            Serial.print("Found address: ");
            Serial.print(i, DEC);
            Serial.print(" (0x");
            Serial.print(i, HEX);
            Serial.println(")");
            count++;
            delay(1);  // maybe unneeded?
        }              // end of good response
    }                  // end of for loop
    Serial.println("Done.");
    Serial.print("Found ");
    Serial.print(count, DEC);
    Serial.println(" device(s).");

    return true;
}
*/



// Requires relayPinArray, relayInitArray, REL_AMOUNT to be declared
bool STB::relay_init(Expander_PCF8574 relay, int pins[], int initvals[], int amount=8) {
    Serial.print(F("\n relay init ... "));
    relay.begin(RELAY_I2C_ADD);
    
    for (int i = 0; i < amount; i++) {
        relay.pinMode(pins[i], OUTPUT);
        relay.digitalWrite(pins[i], initvals[i]);
    }
    Serial.print(F(" successful"));
    return true;
}


/*
 Serial.print("     ");
        Serial.print("Relay ["); Serial.print(relayPinArray[i]); Serial.print("] set to "); Serial.println(relayInitArray[i]);
*/
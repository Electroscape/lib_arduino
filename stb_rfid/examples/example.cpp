
#include <stb_common.h>
#include <stb_rfid.h>


STB STB;


// use Software SPI only, 
// hardware SPI only with SS pin as only arguments has specific problems during first startup
// Pins are define by library and hardware
Adafruit_PN532 RFID_0(PN532_SCK, PN532_MISO, PN532_MOSI, RFID_1_SS_PIN);
Adafruit_PN532 RFID_READERS[1] = {RFID_0};
uint8_t data[16];

#define RFID_DATABLOCK      1

void setup() {
    STB.begin();
    STB.rs485SetSlaveAddr(0);
    STB_RFID::RFIDInit(RFID_0);
}

void loop() {
    // datablock and timeout are optional param
    if (STB_RFID::cardRead(RFID_READERS[0], data, RFID_DATABLOCK)) {
        Serial.println("RFID read succees");
        Serial.println((char*) data);
    }
}
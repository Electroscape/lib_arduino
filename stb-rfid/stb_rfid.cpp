#include "stb-rfid.h"
#include <Adafruit_PN532.h>

const Adafruit_PN532 RFID_0(PN532_SCK, PN532_MISO, PN532_MOSI, RFID_SSPins[0]);
const Adafruit_PN532 RFID_1(PN532_SCK, PN532_MISO, PN532_MOSI, RFID_SSPins[1]);
const Adafruit_PN532 RFID_2(PN532_SCK, PN532_MISO, PN532_MOSI, RFID_SSPins[2]);
const Adafruit_PN532 RFID_3(PN532_SCK, PN532_MISO, PN532_MOSI, RFID_SSPins[3]);

Adafruit_PN532 RFID_READERS[4] = {RFID_0, RFID_1, RFID_2, RFID_3}; //
static Adafruit_PN532 ACTIVE_READERS[];


/**
 * @brief Construct a new stb rfid::stb rfid object
 * 
 * @param activePorts (int) physcial port numbers being used
 * @param keya (uint8_t) key used for the cards encryption usually left at factory FF
 */
STB_RFID::begin(int activePorts[], uint8_t keya[]) {
    for (int port_no = 0, port_no < sizeof(activePorts), port_no++) {
        int port = activePorts[port_no];
        *ACTIVE_READERS[port_no] = *(RFID_READERS[port]);

        Serial.print("initializing reader: "); Serial.println(i);
        ACTIVE_READERS[port_no].begin();
        RFID_READERS[i].setPassiveActivationRetries(5);
            
        int retries = 0;
        while (true) {
            uint32_t versiondata = RFID_READERS[i].getFirmwareVersion();
            if (!versiondata) {
                Serial.println("Didn't find PN53x board");
                // will be done when software reset is in stb-lib
                /*
                if (retries > 5) {
                    Serial.println("PN532 startup timed out, restarting");
                    software_Reset();
                }
                */
            } else {
                Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
                Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
                Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
                break;
            }
            retries++;
        }

    }
}



STB_RFID::read(int reader_nr, int datablock, uint8_t *data, uint8_t *uid, uint8_t uidLength) {
    uint8_t success;
    // wdt_reset();
    // authentication may be shifted to another function if we need to expand
    success = ACTIVE_READERS[reader_nr].mifareclassic_AuthenticateBlock(uid, uidLength, datablock, 0, keya);
    Serial.print("Trying to authenticate block 4 with default KEYA value");
    if (!success) {
        Serial.print("Authentication failed, card may already be authenticated");
    }
    Serial.println();

    success = ACTIVE_READERS[reader_nr].mifareclassic_ReadDataBlock(datablock, data);
    if (!success) {
        Serial.println("Reading failed, discarding card");
        return false;
    }
    return true;
}

#include "stb_rfid.h"

/**
 * @brief 
 * @param reader
 * @return bool
 */
bool STB_RFID::cardDetect(Adafruit_PN532 &reader) {
    uint8_t success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;
    success = RFID_READERS[reader_nr].readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

    return (success && uidLength == 4);
}


/**
 * @brief 
 * 
 * @param reader 
 * @return success
 */
bool STB_RFID::RFID_Init(Adafruit_PN532 &reader) {
    // Serial.print(F("initializing reader..."));

    bool success = true;
    reader.begin();
    success = reader.setPassiveActivationRetries(5);

    int retries = 0;
    while (true) {
        uint32_t versiondata = reader.getFirmwareVersion();
        if (!versiondata) {
            Serial.print(F("Didn't find PN53x board\n"));
            if (retries > 5) {
                Serial.print(F("PN532 startup timed out"));
                return false;
            }
        } else {
            Serial.print(F("Found chip PN5"));
            Serial.println((versiondata >> 24) & 0xFF, HEX);
            Serial.print(F("Firmware ver. "));
            Serial.print((versiondata >> 16) & 0xFF, DEC);
            Serial.print('.');
            Serial.println((versiondata >> 8) & 0xFF, DEC);
            break;
        }
        retries++;
    }
    reader.SAMConfig();
    return true;
}

bool STB_RFID::read_PN532(Adafruit_PN532 &reader, uint8_t *data, uint8_t *uid, int datablock, uint8_t key[]) {
    uint8_t success;
    if (!cardDetect(reader)) {
        return false;
    }
    uint8_t uidLength = 4;
    // 0 is MIFARE_CMD_AUTH_A
    success = reader.mifareclassic_AuthenticateBlock(uid, uidLength, datablock, 0, key);
    return true;
}


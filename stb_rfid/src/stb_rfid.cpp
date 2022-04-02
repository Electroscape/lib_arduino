/**
 * @file stb_rfid.cpp
 * @author Martin Pek (Martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-01
 * 
 */
#include "stb_rfid.h"



/**
 * @brief 
 * 
 * @param reader 
 * @return success
 */
bool STB_RFID::RFIDInit(Adafruit_PN532 &reader) {
    // Serial.print(F("initializing reader..."));

    reader.begin();
    reader.setPassiveActivationRetries(5);

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
    delay(50);
    return true;
}


/**
 * @brief 
 * @param reader
 * @param uid storage space for the uid 
 * @return bool return true for valid mifare classic cards
 */
bool STB_RFID::cardDetect(Adafruit_PN532 &reader, uint8_t *uid) {
    uint8_t success;
    uint8_t uidLength;
    success = reader.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    Serial.println(success);
    Serial.println(uidLength);
    // for (int i=0; i<sizeof(uid); i++) {
    // Serial.print(uid);

    return (success && uidLength == 4);
}

/**
 * @brief 
 * 
 * @param reader 
 * @param data 
 * @param datablock defaults to 1
 * @return true 
 * @return false 
 */
bool STB_RFID::cardRead(Adafruit_PN532 &reader, uint8_t data[16], int datablock=1) {
    uint8_t success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;
    uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    memset(data, 0, sizeof(data));

    success = reader.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (!success) {return false;}

    success = reader.mifareclassic_AuthenticateBlock(uid, uidLength, datablock, 0, keya);
    if (!success) {
        Serial.println("Auth failed"); return false;
    }

    success = reader.mifareclassic_ReadDataBlock(datablock, data);
    if (!success) {
        Serial.println("read failed"); return false;
    }
    if (strlen((char *) data) == 0) {
        Serial.println("read empty datablock -> invalid");
        return false;
    } 
    // Serial.println((char *) data);
    return true;
}

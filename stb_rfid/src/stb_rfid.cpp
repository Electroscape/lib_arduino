/**
 * @file stb_rfid.cpp
 * @author Martin Pek (Martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-01
 * 
 */
#include "stb_rfid.h"
#include <stb_common.h>



/**
 * @brief initializes the parsed RFID reader
 * @param reader 
 * @return success
 */
bool STB_RFID::RFIDInit(Adafruit_PN532 &reader) {

    Serial.print(F("\nRFID init ... \n"));

    reader.begin();
    reader.setPassiveActivationRetries(5);

    int retries = 0;
    while (true) {
        uint32_t versiondata = reader.getFirmwareVersion();
        if (!versiondata) {
            Serial.print(F("Didn't find PN53x board\n"));
            if (retries > 5) {
                Serial.print(F("PN532 startup timed out"));
                delay(5);
    	        STB::softwareReset();
            }
        } else {
            Serial.print(F("Found chip PN5"));
            Serial.println((versiondata >> 24) & 0xFF, HEX);
            Serial.print(F("Firmware ver. "));
            Serial.print((versiondata >> 16) & 0xFF, DEC);
            Serial.print(F("."));
            Serial.println((versiondata >> 8) & 0xFF, DEC);
            break;
        }
        retries++;
    }
    reader.SAMConfig();
    delay(50);
    Serial.print(F("\nRFID init ... Successful \n"));
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
bool STB_RFID::cardRead(Adafruit_PN532 &reader, uint8_t data[16], int datablock, uint16_t timeout) {
    bool success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;
    uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    memset(data, 0, 16);

    success = reader.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, timeout);
    if (!success) {return false;}

    success = reader.mifareclassic_AuthenticateBlock(uid, uidLength, datablock, 0, keya);
    if (!success) {
        Serial.println(F("Auth failed")); return false;
    }

    success = reader.mifareclassic_ReadDataBlock(datablock, data);
    if (!success) {
        Serial.println(F("read failed")); return false;
    }
    if (strlen((char *) data) == 0) {
        Serial.println(F("read empty datablock -> invalid"));
        return false;
    } 
    // Serial.println((char *) data);
    return true;
}


/**
 * @brief is limited to up to 5 characters per reader
 * @param RFID_READERS 
 * @param rfid_amount 
 * @param datablock 
 * @return char 
*/
void STB_RFID::allRFID_Message(Adafruit_PN532 RFID_READERS[4], int rfid_amount, char* inputStr, int datablock) {

    uint8_t data[16];
    strcpy(inputStr, KeywordsList::rfidKeyword.c_str());

    for (int readerNo = 0; readerNo < rfid_amount; readerNo++) {
        if (STB_RFID::cardRead(RFID_READERS[readerNo], data, datablock)) {
            strcat(inputStr, "_");
            strncat(inputStr, (char*) data, 5);
        } else { 
            strcat(inputStr, "_");
            strcat(inputStr, "ZERO");
        }
    }
}



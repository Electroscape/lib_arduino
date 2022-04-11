#pragma once
#include <Adafruit_PN532.h>
#include <PN532_I2C.h>
#include <PN532.h>

#define PN532_SCK               13
#define PN532_MOSI              11
#define PN532_MISO              12

#define RFID_1_SS_PIN           8     /* Per Konvention ist dies RFID-Port 1                                */
#define RFID_2_SS_PIN           7     /* Per Konvention ist dies RFID-Port 2                                */
#define RFID_3_SS_PIN           4     /* Per Konvention ist dies RFID-Port 3                                */
#define RFID_4_SS_PIN           2     /* Per Konvention ist dies RFID-Port 4  */


class STB_RFID {
    public:
    static bool RFIDInit(Adafruit_PN532 &reader);
    static bool cardDetect(Adafruit_PN532 &reader, uint8_t *uid);
    static bool cardRead(Adafruit_PN532 &reader, uint8_t data[16],int datablock);
    static bool RFIDInitI2c(PN532 &reader);
    static bool cardDetectI2c(PN532 &reader, uint8_t *uid);
    static bool cardReadI2c(PN532 &reader, uint8_t data[16],int datablock);
        // static bool RFID_Gate_locked();
        // static bool read_PN532(int reader_nr, uint8_t *data, uint8_t *uid, uint8_t uidLength);
};
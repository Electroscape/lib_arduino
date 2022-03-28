
#include <Adafruit_PN532.h>

#define PN532_SCK               13
#define PN532_MOSI              11
#define PN532_MISO              12

#define RFID_1_SS_PIN           8     /* Per Konvention ist dies RFID-Port 1                                */
#define RFID_2_SS_PIN           7     /* Per Konvention ist dies RFID-Port 2                                */
#define RFID_3_SS_PIN           4     /* Per Konvention ist dies RFID-Port 3                                */
#define RFID_4_SS_PIN           2     /* Per Konvention ist dies RFID-Port 4  */


class STB_RFID {
    public:
    static bool cardDetect(Adafruit_PN532 &reader);
    static bool RFID_Init(Adafruit_PN532 &reader);
    static bool read_PN532(Adafruit_PN532 &reader, uint8_t *data, uint8_t *uid, int datablock, uint8_t key[]);
        // static bool RFID_Gate_locked();
        // static bool read_PN532(int reader_nr, uint8_t *data, uint8_t *uid, uint8_t uidLength);
};
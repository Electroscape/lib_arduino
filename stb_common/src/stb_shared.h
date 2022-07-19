/**
 * @file stb_shared.h
 * @author Martin Pek (martin.pek@web.de)
 * @brief used to store variables used by both Mother and Brain
 * @version 0.1
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */


enum cmdFlags {
    LED,
    RFID,
    OLED
};
 

// global keywords being used
class KeyWords {
    public:
    char flagKeyword[7] = "!Flag_";
};

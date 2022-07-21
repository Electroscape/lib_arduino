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
    OLED,
    // update this everytime you add flags
    AmountOfFlags = 3
};
 

// global keywords being used
class KeywordsList {
    public:
    char flagKeyword[7] = "!Flag_";
    char endFlagKeyword[10] = "!FlagsEnd";
};

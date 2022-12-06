/**
 * @file stb_oled_mother.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief  
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include "stb_oled_mother.h"

void STB_OLED_MOTHER::setOledHeader(char* msg, int brainNo) {
    if (brainNo < 0) {
        // brainNo = Mother.getPolledSlave();
    }
}
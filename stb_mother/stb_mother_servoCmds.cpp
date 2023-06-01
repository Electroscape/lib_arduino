/**
 * @file stb_mother_servoCmds.cpp
 * @author Christian Walter pauseandplay@cybwalt.de
 * @brief  
 * @version 0.1
 * @date 2023-03-04
 * 
 * @copyright Copyright (c) 2023
*/

#include "stb_mother_servoCmds.h"



/**
 * @brief sets the given brain to the given clr
 * @param STB_MOTHER 
 * @param brainNo 
 * @param clr 
 * @param ledCnt default is all Leds
 */
void SERVO_CMDS::moveServo(STB_MOTHER &Mother, int brainNo, int stripNo, int pos) {
    char msg[48] = "";
    char noString[3] = "";
    
    strcpy(msg, KeywordsList::ServoKeyword.c_str());
    sprintf(noString, "%i", servoCmds::moveServo);
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    sprintf(noString, "%i", stripNo);
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    sprintf(noString, "%i", pos);
    strcat(msg, noString);

    Mother.sendCmdToSlave(msg, brainNo);
}


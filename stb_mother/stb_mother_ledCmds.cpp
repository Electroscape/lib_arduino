/**
 * @file stb_mother_ledCmds.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief  
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright Copyright (c) 2022
 * TODO: 
 * - implement adding the clr to the msg as a fnc
*/

#include "stb_mother_ledCmds.h"


constexpr int LED_CMDS::clrWhite[3];
constexpr int LED_CMDS::clrRed[3];
constexpr int LED_CMDS::clrGreen[3];
constexpr int LED_CMDS::clrBlack[3];
constexpr int LED_CMDS::clrBlue[3];
constexpr int LED_CMDS::clrYellow[3];



/**
 * @brief sets the given brain to the given clr
 * @param STB_MOTHER 
 * @param brainNo 
 * @param clr 
 * @param ledCnt default is all Leds
 */
void LED_CMDS::setAllStripsToClr(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness, int ledCnt) {
    char msg[32] = "";
    char noString[3] = "";
    sprintf(noString, "%i", ledCmds::setAll);
    
    strcpy(msg, KeywordsList::ledKeyword.c_str());
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    // repeat function ... pack this into a fnc
    for (int i=0; i<3; i++) {
        char intStr[3];
        itoa((clr[i] * brightness) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }

    Mother.sendCmdToSlave(msg, brainNo);
}


/**
 * @brief  sets a single given pixel to 
 * @param Mother 
 * @param pixel 
 * @param clr 
 * @param brightness 
 * @param brainNo 
*/
void LED_CMDS::setStripToClr(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness, int stripNo) {
    char msg[32] = "";
    char noString[3] = "";
    sprintf(noString, "%i", ledCmds::setStripToClr);

    strcpy(msg, KeywordsList::ledKeyword.c_str());
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    sprintf(noString, "%i", stripNo);
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());

    // repeat function ... pack this into a fnc
    for (int i=0; i<3; i++) {
        char intStr[3];
        itoa((clr[i] * brightness) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }

    Mother.sendCmdToSlave(msg, brainNo);
}

/**
 * @brief switches off all LEDStrips on the given brain
 * @param STB_MOTHER
 * @param brainNo 
 * @param ledCnt default is all Leds
 */
void LED_CMDS::turnOff(STB_MOTHER &Mother, int brainNo, int ledCnt) {
    setAllStripsToClr(Mother, brainNo, clrBlack, int(100), ledCnt);
}

/**
 * @brief set the LED strip to running light with defined color clr
 * @param STB_MOTHER 
 * @param brainNo 
 * @param clr 
 * @param actLED how much LEDs are active 
 */
void LED_CMDS::running(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness, int runTime, int actLED) {
    char msg[32] = "";
    char noString[3] = "";
    sprintf(noString, "%i", ledCmds::setRunning);
    
    strcpy(msg, KeywordsList::ledKeyword.c_str());
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    // repeat function ... pack this into a fnc
    
    for (int i=0; i<3; i++) {
        char intStr[3];        
        itoa((clr[i] * brightness) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }

    char intStr[5]; 
    strcat(msg, "_");
    itoa(runTime, intStr, 10);
    strcat(msg,intStr );
    strcat(msg, "_");
    itoa(actLED, intStr, 10);
    strcat(msg,intStr );

    Mother.sendCmdToSlave(msg, brainNo);
    
    //Serial.println(F("Running send"));
}

/**
 * @brief starts a running light sequence over the PWMs
 * @param STB_MOTHER 
 * @param brainNo 
 * @param clr 
 * @param actLED how much LEDs are active 
 */

void LED_CMDS::runningPWM(STB_MOTHER &Mother, int brainNo, const int clr[3], int brightness, int runTime, int actPWM) {
    char msg[32] = "";
    char noString[3] = "";
    sprintf(noString, "%i", ledCmds::setRunningPWM);
    
    strcpy(msg, KeywordsList::ledKeyword.c_str());
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    // repeat function ... pack this into a fnc
    
    for (int i=0; i<3; i++) {
        char intStr[3];            
        itoa((clr[i] * brightness) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }

    char intStr[5]; 
    strcat(msg, "_");
    itoa(runTime, intStr, 10);
    strcat(msg,intStr );
    strcat(msg, "_");
    itoa(actPWM, intStr, 10);
    strcat(msg,intStr );

    Mother.sendCmdToSlave(msg, brainNo);
    
    //Serial.println(F("Running send"));
}
  

/**
 * @brief set the LED strip to blinking with two different colors and different times
 * @param STB_MOTHER 
 * @param brainNo 
 * @param clr1 
 * @param clr2
 * @param blinkTime1
 * @param blinkTime2
 * @param Strip_No
 */
    void LED_CMDS::blinking(STB_MOTHER &Mother, int brainNo, const int clr1[3], const int clr2[3], int blinkTime1, int blinkTime2, int brightness1,  int brightness2,int stripNo) {
    char msg[32] = "";
    char noString[3] = "";
    sprintf(noString, "%i", ledCmds::setBlinking);
    
    strcpy(msg, KeywordsList::ledKeyword.c_str());
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    
    char intStr[5]; 
    itoa(stripNo, intStr, 10);
    strcat(msg, intStr );
    strcat(msg, KeywordsList::delimiter.c_str());

    for (int i=0; i<3; i++) {
        char intStr[3];        
        itoa((clr1[i] * brightness1) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }
    strcat(msg, "_");

    
    for (int i=0; i<3; i++) {
        char intStr[3];        
        itoa((clr2[i] * brightness2) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }
    
    strcat(msg, "_");
    itoa(blinkTime1, intStr, 10);
    strcat(msg,intStr );

    strcat(msg, "_");
    itoa(blinkTime2, intStr, 10);
    strcat(msg,intStr );


   
    Mother.sendCmdToSlave(msg, brainNo);
    
    //Serial.println(F("Blinking Send"));

    }

/**
 * @brief 
 * 
 * @param Mother 
 * @param brainNo 
 * @param color1 
 * @param brightness1 
 * @param color2 
 * @param brightness2 
 * @param runTime 
 * @param stripNo 
 * @return * void 
 */
    void LED_CMDS::fade2color(STB_MOTHER &Mother, int brainNo,  const int clr1[3], int brightness1, const int clr2[3], int brightness2, int runTime, int stripNo) {


    char msg[32] = "";
    char noString[3] = "";
    sprintf(noString, "%i", ledCmds::setfade2color);
    
    strcpy(msg, KeywordsList::ledKeyword.c_str());
    strcat(msg, noString);
    strcat(msg, KeywordsList::delimiter.c_str());
    
    char intStr[5]; 
    
    itoa(stripNo, intStr, 10);
    strcat(msg,intStr );
    strcat(msg, "_");

    
    for (int i=0; i<3; i++) {
        char intStr[3];        
        itoa((clr1[i] * brightness1) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }
    strcat(msg, "_");

    
    for (int i=0; i<3; i++) {
        char intStr[3];        
        itoa((clr2[i] * brightness2) / 100, intStr, 10);
        strcat(msg, intStr);
        if (i<2) {strcat(msg, "_");}
    }
    
    strcat(msg, "_");
    itoa(runTime, intStr, 10);
    strcat(msg,intStr );

    Mother.sendCmdToSlave(msg, brainNo);
    }
/**
 * @file stb_led.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-01
 * TODO: 
 * - only works for single digits of pixels, evaluation fnc need reevaluation
 */


#include "stb_led.h"
#include "string.h"



STB_LED::STB_LED() {};

unsigned long lightTimming = millis();

// due to compile time requirements from some values
void STB_LED::enableStrip0(uint32_t clrOrder) {
    delay(10);
    // uint32_t clrOrder=NEO_BRG; 
    // int clkSpeed=NEO_KHZ800;
    int i = 0;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (clrOrder + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip1(uint32_t clrOrder) {
    delay(10);
    // uint32_t clrOrder=NEO_BRG; 
    // int clkSpeed=NEO_KHZ800;
    int i = 1;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (clrOrder + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip2(uint32_t clrOrder) {
    delay(10);
    // uint32_t clrOrder=NEO_BRG; 
    // int clkSpeed=NEO_KHZ800;
    int i = 2;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (clrOrder + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip3(uint32_t clrOrder) {
    delay(10);
    // uint32_t clrOrder=NEO_BRG; 
    // int clkSpeed=NEO_KHZ800;
    int i = 3;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (clrOrder + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


/**
 * @brief 
 * 
 * @param neopixel array of type Adafruit_NeoPixel
 * @param ledCnts int array delcaring how many leds are on said strip
 * @param pins  int array with the pins to be used
 * @param clrOrder defaults to NEO_BRG
 * @param clkSpeed defaults to NEO_KHZ800
 * @return true 
 * @return false 
 * Todo: prevent this from possibly being stuck when wrong data is rcvd
 */
bool STB_LED::ledInit(int settings[SETTINGS_CNT][SETTINGS_PARAMS], uint32_t clrOrder, int clkSpeed) {
    
    Serial.print(F("\n\nLED init ..."));
 
    int row = 0;
    int clrOrdersRow = -1;
    while (row < SETTINGS_CNT && settings[row][0] >= 0) {
        if (settings[row][0] == settingCmds::ledCount) {
            int stripeNo = settings[row][1];
            // safety checks otherwise we go out of index
            if (stripeNo < 0 || stripeNo >= STRIPE_CNT) { continue; }
            // if (LED_MAX_CNT < settings[row][2]) { continue; }

            activeLeds[stripeNo] = (uint16_t) settings[row][2];
        } else if (settings[row][0] == settingCmds::ledClrOrder) {
           clrOrdersRow = row;
        }
        row++;
    }

    if (clrOrdersRow < 0) {
        Serial.println(F("missing clr Order"));
        return false;
    }

    for (int i=0; i<STRIPE_CNT; i++) {
        if (activeLeds[i] <= 0) { continue; }
        switch (i) {
            case 0: enableStrip0((uint32_t) settings[clrOrdersRow][i+1]); break;
            case 1: enableStrip1((uint32_t) settings[clrOrdersRow][i+1]); break;
            case 2: enableStrip2((uint32_t) settings[clrOrdersRow][i+1]); break;
            case 3: enableStrip3((uint32_t) settings[clrOrdersRow][i+1]); break;
        }
    }
    setAllStripsToClr(Strips[0].Color(0,0,0));
    Serial.println(F(" successful"));

    return true;
}


/**
 * @brief 
 * @param stripNo index from 0-3
 * @param clr 
 */
void STB_LED::setStripToClr(int stripNo, long int clr, bool show) {
    if ((int) activeLeds[stripNo] <= 0) { return; }
    for (uint16_t ledNr=0; ledNr<activeLeds[stripNo]; ledNr++) {
        Strips[stripNo].setPixelColor(ledNr, clr);
    }
    if (show) {Strips[stripNo].show();}
    delay(1);
}
void STB_LED::setLEDToClr(int stripNo,uint16_t ledNr, long int clr) {
   
    Strips[stripNo].setPixelColor(ledNr, clr);
    Strips[stripNo].show();
}


/**
 * @brief 
 * 
 * @param neopixel 
 * @param clrs 
void STB_LED::setStripToClrs(int stripNo, uint32_t  clrs[], int size) {
    for (int ledNr=0; ledNr<size; ledNr++) {
        Strips[stripNo].setPixelColor(ledNr, clrs[ledNr]);
        Strips[stripNo].show();
    }
    delay(10);
}
*/


/**
 * @brief 
 * 
 * @param neopixels array of led instances
 * @param clr 
 */
void STB_LED::setAllStripsToClr(long int clr) {
    for (int i=0; i<STRIPE_CNT; i++) {
        // the check if the strip is activated is done in the following fnc
        setStripToClr(i, clr, false);
    }
    for (int i=0; i<STRIPE_CNT; i++) {
        Strips[i].show();
    }
}

void STB_LED::loop(STB_BRAIN &Brain){
    // only for 1 PWM make it flexible
        switch( lightState[0]){

            case setRunning:running(0,color1,runTime,actLED);break;
            case setRunningPWM: runningPWM(color1,runTime,actLED); break;
        }


}




/**
 * @brief runningLight
 * 
 * @param neopixels array of led instances
 * @param clr 
 */
void STB_LED::running(int stripNo, long int clr, int runTime, int actLED) {
    // only for strip with 6 LED, needs edit
    lightState[0] = setRunning;
    unsigned long actTime = millis() - lightTimming;
    if (actTime > runTime){
        lightTimming = millis();
        actTime = 0;
    }

    long black =Strips[0].Color(0,0,0);
    uint16_t nrLED = uint16_t(round(float(actTime)/float(runTime)*float(6))-1);
    if (nrLED>6){nrLED = 6;}
    uint16_t nrOUT = nrLED-1;
    if (nrLED==0){
        nrOUT = 5;
    }
    

    setLEDToClr(stripNo, nrLED, clr);
    setLEDToClr(stripNo, nrOUT, black);
    
}
/**
 * @brief runningLight
 * 
 * @param neopixels array of led instances
 * @param clr 
 */
void STB_LED::runningPWM(long int clr,int runTime,int actPWM) {
    for (int i = 0; i<= actPWM; i++){
        lightState[i] = setRunningPWM;
    }
    unsigned long actTime = millis()-lightTimming;
    if (actTime>runTime){
        lightTimming = millis();
        actTime = 0;
    }

    long black =Strips[0].Color(0,0,0);
    uint16_t nrLED = uint16_t(round(float(actTime)/float(runTime)*float(actPWM))-1);
    if (nrLED>actPWM-1){nrLED = actPWM-1;}
    uint16_t nrOUT = nrLED-1;
    if (nrLED==0){
        nrOUT = actPWM-1;
    }
    
    setStripToClr(nrLED,clr);
    setStripToClr(nrOUT,black);

    
}


void STB_LED::blinking(int stripNo,const int clr1,const int clr2,int blinkTime1, int blinkTime2) {
    
    //Serial.println("Blinking");
    lightState[0] = setBlinking;
       
}


/**
 * @brief  
 * @param Brain 
 * @return if cmd was found and evaluated
*/
bool STB_LED::evaluateCmds(STB_BRAIN &Brain) {
    if (strncmp(KeywordsList::ledKeyword.c_str(), Brain.STB_.rcvdPtr, KeywordsList::ledKeyword.length()) != 0) {
        return false;
    }
    // Serial.println("received ledKeyword");
    Brain.STB_.rcvdPtr += KeywordsList::ledKeyword.length();

    // Serial.println(Brain.STB_.rcvdPtr);
    int cmdNo;
    if (sscanf(Brain.STB_.rcvdPtr, "%d", &cmdNo) <= 0) { return false; } 
    Brain.STB_.rcvdPtr += 2;
    long int setClr;
    int value; 
    // uint32_t currentClr;
   
    
    //Serial.println(cmdNo);
    int clrs[3];
    switch (cmdNo) {
        case setAll:
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            setAllStripsToClr(setClr);
        break;
        case ledCmds::setStripToClr:
            if (sscanf(Brain.STB_.rcvdPtr, "%d", &value) <= 0) { return false; } 
            Brain.STB_.rcvdPtr += 2; // only works for single digits of pixels
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            // && i< pixelNo
            setStripToClr(value, setClr);
        break;

        case setRunning: // starts the runningLight sequence
            getBufferValues(Brain, 3, *clrs);
            getBufferValues(Brain, 1, runTime);
            getBufferValues(Brain, 1, actLED);

            running(0, color1, runTime, actLED);
        break;

        case setRunningPWM: // starts the runningLight sequence
            //getBufferValues(Brain,3,*clrs); // soll auch mit mehreren Werten klappen
            getBufferValues(Brain, 1, clrs[0]);
            getBufferValues(Brain, 1, clrs[1]);
            getBufferValues(Brain, 1, clrs[2]);
            color1 =  Strips[0].Color(clrs[0], clrs[1], clrs[2]);  
            getBufferValues(Brain, 1, runTime);
            getBufferValues(Brain, 1, actLED);
            runningPWM(color1, runTime, actLED);
        break;
        
        case setBlinking:  // starts the blinking sequence
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            blinking(0, setClr, setClr, 500, 500);
        break;

         
        case setDimming:  // starts the dimming sequence
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            //dimming(setClr);
        break;

        default: return false;
    }
    return true;
}


/**
 * @brief reads clr data from buffer and writes information into setClr
 * @param Brain 
 * @param clrs 
*/
bool STB_LED::getClrsFromBuffer(STB_BRAIN &Brain, long int &setClr) {
    int clrs[3];

    if (sscanf(Brain.STB_.rcvdPtr,"%d_%d_%d", &clrs[0], &clrs[1], &clrs[2]) != 3) {
        // Serial.println("did not get value from");
        return false;
    }
    setClr = Strips[0].Color(clrs[0], clrs[2], clrs[1]);
    // everything needed has been read, hence answering the mother can be done here already
    Brain.sendAck();
    return true;
}

bool STB_LED::getBufferValues(STB_BRAIN &Brain, int nrValues,  int &values){
    
    int tmpValues[5];
    
    for (int i=0; i <=nrValues-1;i++){
        sscanf(Brain.STB_.rcvdPtr,"%d", &tmpValues[i]); //gets Value No i from Buffer  
        Brain.STB_.rcvdPtr=strstr( Brain.STB_.rcvdPtr, "_")+1; // find Delimiter in String and move pointer
        //Serial.println(tmpValues[i]);
    }

    values = *tmpValues;
    return true;

}
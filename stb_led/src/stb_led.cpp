/**
 * @file stb_led.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-01
 * TODO: 
 * - only works for single digits of pixels, evaluation fnc need reevaluation
 * - passing of clr order and freq to enableStrips 
 */


#include "stb_led.h"
#include "string.h"



STB_LED::STB_LED() {};

unsigned long lightTimming=millis();


void STB_LED::enableStrip0() {
    delay(10);
    Serial.println("STB_LED::enableStrip0");
    // uint32_t clrOrder=NEO_BRG; 
    // int clkSpeed=NEO_KHZ800;
    int i = 0;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (NEO_BRG + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip1() {
    delay(10);
    Serial.println("STB_LED::enableStrip1");
    // uint32_t clrOrder=NEO_BRG; 
    // int clkSpeed=NEO_KHZ800;
    int i = 1;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (NEO_BRG + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip2() {
    delay(10);
    Serial.println("STB_LED::enableStrip2");
    // uint32_t clrOrder=NEO_BRG; 
    // int clkSpeed=NEO_KHZ800;
    int i = 2;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (NEO_BRG + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip3() {
    delay(10);
    Serial.println("STB_LED::enableStrip3");
    // uint32_t clrOrder=NEO_BRG; 
    // int clkSpeed=NEO_KHZ800;
    int i = 3;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (NEO_BRG + NEO_KHZ800));
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
    while (row < SETTINGS_CNT && settings[row][0] >= 0) {
        if (settings[row][0] == settingCmds::ledCount) {
            int stripeNo = settings[row][1];
            // safety checks otherwise we go out of index
            if (stripeNo < 0 || stripeNo >= STRIPE_CNT) { continue; }
            // if (LED_MAX_CNT < settings[row][2]) { continue; }

            activeLeds[stripeNo] = (uint16_t) settings[row][2];
        }
        row++;
    }


    for (int i=0; i<STRIPE_CNT; i++) {
        if (activeLeds[i] <= 0) { continue; }
        switch (i) {
            case 0: enableStrip0(); break;
            case 1: enableStrip1(); break;
            case 2: enableStrip2(); break;
            case 3: enableStrip3(); break;
        }
    }
    setAllStripsToClr(Strips[0].Color(0,0,0));
    Serial.println(F(" successful"));

    /*
    Serial.print("enabled so many leds: ");
    Serial.print(String(activeLeds[0]));
    Serial.print(" ");
    Serial.print(String(activeLeds[1]));
    Serial.print(" ");
    Serial.print(String(activeLeds[2]));
    Serial.print(" ");
    Serial.print(String(activeLeds[3]));
    */

    return true;
}

/**
 * @brief 
 * @param stripNo index from 0-3
 * @param clr 
 */
void STB_LED::setStripToClr(int stripNo, long int clr) {
   // Serial.println(String(activeLeds[stripNo]));
    if ((int) activeLeds[stripNo] <= 0) { return; }
    /* Serial.print("setting stripNo ");
    Serial.println(String(stripNo)); */
    for (uint16_t ledNr=0; ledNr<activeLeds[stripNo]; ledNr++) {
        Strips[stripNo].setPixelColor(ledNr, clr);
    }
    Strips[stripNo].show();
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
 */
void STB_LED::setStripToClrs(int stripNo, uint32_t  clrs[], int size) {
    for (int ledNr=0; ledNr<size; ledNr++) {
        Strips[stripNo].setPixelColor(ledNr, clrs[ledNr]);
        Strips[stripNo].show();
    }
    delay(10);
}
/**
 * @brief 
 * 
 * @param neopixels array of led instances
 * @param clr 
 */
void STB_LED::setAllStripsToClr(long int clr) {
    for (int i=0; i<STRIPE_CNT; i++) {
        // the check if the strip is activated is done in the following fnc
        setStripToClr(i, clr);
    }
}

void STB_LED::loop(STB_BRAIN &Brain){
    /*  Serial.println("Run LED loop");
    Serial.println(lightState[0]);  */
    //for (int i_PWM=0; i_PWM<=STRIPE_CNT; i_PWM++) // only for 4 PWM make it flexible
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
void STB_LED::running(int stripNo,long int clr,int runTime,int actLED) {
    lightState[0] = setRunning;
    unsigned long actTime = millis()-lightTimming;
    if (actTime>runTime){
        lightTimming = millis();
        actTime = 0;
    }

    long gray =Strips[0].Color(40,40,40);
    long black =Strips[0].Color(0,0,0);
    uint16_t nrLED = uint16_t(round(float(actTime)/float(runTime)*float(6))-1);
    if (nrLED>6){nrLED = 6;}
    uint16_t nrHALF = nrLED-1;
    uint16_t nrOUT = nrLED-2;
    if (nrLED==0){
        nrHALF =5;
        nrOUT = 4;
    }
    if (nrLED==1){
        nrOUT = 5;
    }
    /* Serial.println("Running");
    Serial.println(nrLED);
    Serial.println(actTime);
    Serial.println(lightTimming);
    Serial.println(runTime); */


    setLEDToClr(stripNo,nrLED,clr);
    setLEDToClr(stripNo,nrHALF,black);
    setLEDToClr(stripNo,nrOUT,black);
    
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
    Serial.println("received ledKeyword");
    Brain.STB_.rcvdPtr += KeywordsList::ledKeyword.length();

    //Serial.println(Brain.STB_.rcvdPtr);
    int cmdNo;
    if (sscanf(Brain.STB_.rcvdPtr, "%d", &cmdNo) <= 0) { return false; } 
    Brain.STB_.rcvdPtr += 2;
    long int setClr;
    int pixelInt; 
    uint16_t pixelNo;
    // uint32_t currentClr;
   
    
    //Serial.println(cmdNo);
    int clrs[3];
    switch (cmdNo) {
        case setAll:
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            setAllStripsToClr(setClr);
        break;
        case setPixel:

            if (sscanf(Brain.STB_.rcvdPtr, "%d", &pixelInt) <= 0) { return false; } 
            Brain.STB_.rcvdPtr += 2; // only works for single digits of pixels
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }

            pixelNo = (uint16_t) pixelInt;
            // && i< pixelNo
            for (uint16_t i=0; (i<activeLeds[0]); i++) {
                if (pixelNo == i) {
                    Strips[0].setPixelColor(pixelNo, setClr);
                } else {
                    Strips[0].setPixelColor(i, Strips[0].Color(127,0,0));
                }
            }
            Strips[0].show();
        break;

        case setRunning: // starts the runningLight sequence
            getBufferValues(Brain,3,*clrs);
            getBufferValues(Brain,1,runTime);
            getBufferValues(Brain,1,actLED);

            running(0,color1,runTime,actLED);
        break;

        case setRunningPWM: // starts the runningLight sequence
            //getBufferValues(Brain,3,*clrs); // soll auch mit mehreren Werten klappen
            getBufferValues(Brain,1,clrs[0]);
            getBufferValues(Brain,1,clrs[1]);
            getBufferValues(Brain,1,clrs[2]);
            color1 =  Strips[0].Color(clrs[0],clrs[1],clrs[2]);  
            //Serial.println(clrs[0]);Serial.println(clrs[1]); Serial.println(clrs[2]);
            getBufferValues(Brain,1,runTime);
            getBufferValues(Brain,1,actLED);
            /* Serial.print(clrs[0]);
            Serial.print(clrs[1]);
            Serial.println(clrs[2]); */
            runningPWM(color1,runTime,actLED);
        break;
        
        case setBlinking:  // starts the blinking sequence
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            blinking(0,setClr,setClr,500,500);
        break;

         
        case setDimming:  // starts the dimming sequence
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            //dimming(setClr);
        break;

        default: return false;
    }
    // how fast is evaluation and setting?
    Brain.sendAck();
    return true;
}


/**
 * @brief reads clr data from buffer and writes as clr
 * @param Brain 
 * @param clrs 
*/
bool STB_LED::getClrsFromBuffer(STB_BRAIN &Brain, long int &setClr) {
    int clrs[3];

    if (sscanf(Brain.STB_.rcvdPtr,"%d_%d_%d", &clrs[0], &clrs[1], &clrs[2]) != 3) {
        Serial.println("did not get value from");

        return false;
    }
    setClr = Strips[0].Color(clrs[0],clrs[2],clrs[1]);
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
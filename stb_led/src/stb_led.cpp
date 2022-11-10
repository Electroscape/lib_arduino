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



STB_LED::STB_LED() {};


// due to compile time requirements from some values
void STB_LED::enableStrip0(uint32_t clrOrder) {
    delay(10);
    int i = 0;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (clrOrder + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip1(uint32_t clrOrder) {
    delay(10);
    int i = 1;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (clrOrder + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip2(uint32_t clrOrder) {
    delay(10);
    int i = 2;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (clrOrder + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


void STB_LED::enableStrip3(uint32_t clrOrder) {
    delay(10);
    int i = 3;
    Strips[i] = Adafruit_NeoPixel((uint16_t) LED_MAX_CNT, ledPins[i], (clrOrder + NEO_KHZ800));
    Strips[i].begin();
    delay(10);
}


/**
 * @brief 
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


/**
 * @brief sets a single led to a given clr
 * @param stripNo 
 * @param ledNr 
 * @param clr 
*/
void STB_LED::setLEDToClr(int stripNo, uint16_t ledNr, long int clr) {  
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


/**
 * @brief to execute advanced led commands, add this in the main loop
 * @param Brain 
*/
void STB_LED::LEDloop(STB_BRAIN &Brain){
    for (int i = 0; i < STRIPE_CNT; i++) {
        switch (lightState[i]) {
            case ledCmds::setRunning: running(i); break;
            case ledCmds::setRunningPWM: runningPWM(); break;
            case ledCmds::setBlinking: blinking(i); break;
            case ledCmds::setfade2color: fade2color(i); break;
        }
    }
}




/**
 * @brief runningLight
 * 
 * @param stripNo 
 */
void STB_LED::running(int stripNo) {
    

    if (lightTiming[stripNo] > millis()) { return; }       // return if lightTiming is not reached
	
    lightTiming[stripNo] += deltaTime[stripNo];                 // set new timepoint
    setLEDToClr(stripNo,LED_ON[stripNo], clrBlack);             // turn off LED
    LED_ON[stripNo] += 1;                                 // set new LED
    
    if (LED_ON[stripNo] > uint16_t(usedLED[stripNo]) - 1) { // condition if LED_ON reaches end of actPWM

        if (pauseTime[stripNo] > 0 && LED_ON[stripNo] != 314){                            // check if Pause is active
            lightTiming[stripNo] = lightTiming[stripNo] - deltaTime[stripNo] + pauseTime[stripNo];     // reset timing with defined pauseTime
            LED_ON[stripNo] = 313;                                                // Indictor for Pause, negativ not possible because of unsigned...
        }
        else{
            LED_ON[stripNo] = 0;
        } 
    }    
    setLEDToClr(stripNo,LED_ON[stripNo], colorLEDLoop[stripNo][0]);            // turn on new LED       
}


/**
 * @brief runningPWM
 */
void STB_LED::runningPWM() { //(CW 281022)
   
    if (lightTiming[0] > millis()) { return; }       // return if lightTiming is not reached

    uint16_t actPWM = usedLED[0];                    // rename 	

    lightTiming[0] += deltaTime[0];                 // set new timepoint
    setStripToClr(LED_ON[0], clrBlack);             // turn off LED
    LED_ON[0] += 1;                                 // set new LED
   // Serial.println(LED_ON[0]);

    if (LED_ON[0] > actPWM-1) { // condition if LED_ON reaches end of actPWM

         if (pauseTime[0] > 0 && LED_ON[0] != 314){                            // check if Pause is active
            lightTiming[0] = lightTiming[0] - deltaTime[0] + pauseTime[0];     // reset timing with defined pauseTime
            LED_ON[0] = 313;                                                // Indictor for Pause, negativ not possible because of unsigned...
        }
        else{
            LED_ON[0] = 0;
        } 
    }    

    setStripToClr(LED_ON[0], color1[0]);            // turn on new LED
}


/**
 * @brief WIP @Christian walter
 * @param stripNo 
*/
void STB_LED::blinking(int stripNo) { //(CW 281022)
  
    if (lightTiming[stripNo]> millis()) { return; } 
    
    if(LED_ON[stripNo] == 0){
        lightTiming[stripNo] += blinkTime1[stripNo];
        LED_ON[stripNo] = 1;
        setStripToClr(stripNo, color1[stripNo]);             // turn off LED
    }
    else{
        lightTiming[stripNo] += blinkTime2[stripNo];
        LED_ON[stripNo] = 0;
        setStripToClr(stripNo, color2[stripNo]);
    }
}


/**
 * @brief 
 * 
 * 
 * @param stripNo 
 */
void STB_LED::fade2color(int stripNo){

    unsigned long actTime = millis();

    if (lightTiming[stripNo] < actTime) { // last call force color
        lightState[stripNo] = -1;          
        setStripToClr(stripNo, color2[stripNo]);    
        return; 
    } 

    uint8_t clr1_1 = color1[stripNo] >> 16; //bit shift 16 (Adafruit_NeoPixel::color)
    uint8_t clr1_2 = color1[stripNo]; //bit shift 8
    uint8_t clr1_3 = color1[stripNo] >> 8; //bit shift 0

    uint8_t clr2_1 = color2[stripNo] >> 16; //bit shift 16 (Adafruit_NeoPixel::color)
    uint8_t clr2_2 = color2[stripNo]; //bit shift 8
    uint8_t clr2_3 = color2[stripNo] >> 8; //bit shift 0
    

    // linear interpolation of new color    
    unsigned long t0 = lightTiming[stripNo]- runTime[stripNo];
    unsigned long t1 = lightTiming[stripNo];

    uint8_t newclr_1 = clr1_1*(t1 - actTime)/runTime[stripNo] + clr2_1*( actTime - t0)/runTime[stripNo];
    uint8_t newclr_2 = clr1_2*(t1 - actTime)/runTime[stripNo] + clr2_2*( actTime - t0)/runTime[stripNo];
    uint8_t newclr_3 = clr1_3*(t1 - actTime)/runTime[stripNo] + clr2_3*( actTime - t0)/runTime[stripNo];
    
    setStripToClr(stripNo, Adafruit_NeoPixel::Color(newclr_1, newclr_3, newclr_2));
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

    //Serial.println(Brain.STB_.rcvdPtr);
    int cmdNo;
    if (sscanf(Brain.STB_.rcvdPtr, "%d", &cmdNo) <= 0) {return false; } 
    Brain.STB_.rcvdPtr += 2;
    long int setClr;
    int value; 
    // uint32_t currentClr;
   
     
    //Serial.println(cmdNo);
    int clrs[3];
    int stripValue;
    int stripNr;
    //for (int i=0; i < STRIPE_CNT; i++) {lightState[i] = -1;}

    switch (cmdNo) {
        case setAll:
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            setAllStripsToClr(setClr);
            for (int i=0; i < STRIPE_CNT; i++) {lightState[i] = -1;}

        break;
        case ledCmds::setStripToClr:
            if (sscanf(Brain.STB_.rcvdPtr, "%d", &value) <= 0) { return false; } 
            Brain.STB_.rcvdPtr += 2; // only works for single digits of pixels
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            // && i< pixelNo
            setStripToClr(value, setClr);
            lightState[value] = -1;
        break;

        case setRunning: // starts the runningLight sequence 
            getBufferValues(Brain, 1, stripValue);   
            switch(stripValue){ // combinded blinking has to be added
                case 1: stripNr = 0; break;
                case 2: stripNr = 1; break;
                case 4: stripNr = 2; break;
                case 8: stripNr = 3; break;
            }    
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            getBufferValues(Brain, 1, runTime[stripNr]);
            getBufferValues(Brain, 1, usedLED[stripNr]);
            getBufferValues(Brain, 1, pauseTime[stripNr]);
            Brain.sendAck();
            colorLEDLoop[stripNr][0] =  STB_LED::Strips->Color(clrs[0], clrs[2], clrs[1]);  
            lightState[stripNr] = ledCmds::setRunning;  //for PWM only the first state is necessary  
            deltaTime[stripNr] =  long(round(long(runTime[stripNr]) / long(usedLED[stripNr])));
            lightTiming[stripNr] = millis() + deltaTime[stripNr];
            LED_ON[stripNr] = 0;
            setLEDToClr(stripNr, LED_ON[stripNr], color1[stripNr]);            // turn on new LED
            running(stripNr);

        break;

        case setRunningPWM: // starts the runningLight sequence (CW 281022)
            //getBufferValues(Brain,3,*clrs); // soll auch mit mehreren Werten klappen
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            getBufferValues(Brain, 1, runTime[0]);
            getBufferValues(Brain, 1, usedLED[0]);
            getBufferValues(Brain, 1, pauseTime[0]);
            Brain.sendAck();
            color1[0] =  STB_LED::Strips->Color(clrs[0], clrs[2], clrs[1]);  
            lightState[0] = setRunningPWM;  //for PWM only the first state is necessary  
            deltaTime[0] =  long(round(long(runTime[0])/long(usedLED[0])));
            lightTiming[0] = millis()+deltaTime[0];
            LED_ON[0] = 0;
            setStripToClr(LED_ON[0], color1[0]);            // turn on new LED
            runningPWM();
            
        break;
        
        case setBlinking:  // starts the blinking sequence
            getBufferValues(Brain, 1, stripValue);   
            switch(stripValue){ // combinded blinking has to be added
                case 1: stripNr = 0; break;
                case 2: stripNr = 1; break;
                case 4: stripNr = 2; break;
                case 8: stripNr = 3; break;
            }    
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            color1[stripNr] =  STB_LED::Strips->Color(clrs[0], clrs[2], clrs[1]);  
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            color2[stripNr] =  STB_LED::Strips->Color(clrs[0], clrs[2], clrs[1]);  
            getBufferValues(Brain, 1, blinkTime1[stripNr]); getBufferValues(Brain, 1, blinkTime2[stripNr]);   
            Brain.sendAck();

            lightState[stripNr] = setBlinking; 
            lightTiming[stripNr] = millis();
            blinking(stripNr);
        break;

        case setfade2color: // fading to new color

            getBufferValues(Brain, 1, stripValue);   
               
            switch(stripValue){ // combinded blinking has to be added
                case 1: stripNr = 0; break;
                case 2: stripNr = 1; break;
                case 4: stripNr = 2; break;
                case 8: stripNr = 3; break;
            }    
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            color1[stripNr] =  STB_LED::Strips->Color(clrs[0], clrs[2], clrs[1]);  
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            color2[stripNr] =  STB_LED::Strips->Color(clrs[0], clrs[2], clrs[1]);  
            getBufferValues(Brain, 1, runTime[stripNr]);  
            Brain.sendAck();
            lightState[stripNr] = ledCmds::setfade2color; 
            lightTiming[stripNr] = millis() + runTime[stripNr];       
            fade2color(stripNr);


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


/**
 * @brief  
 * @param Brain 
 * @param nrValues 
 * @param values 
 * @return ALWAYS TRUE @Christian Walter may aswell make it void or return false if sscanf doesnt find values
*/
bool STB_LED::getBufferValues(STB_BRAIN &Brain, int nrValues,  int &values){
    
    int tmpValues[5];
    
    for (int i=0; i <=nrValues-1;i++){
        sscanf(Brain.STB_.rcvdPtr,"%d", &tmpValues[i]); //gets Value No i from Buffer  
        Brain.STB_.rcvdPtr=strstr( Brain.STB_.rcvdPtr, "_")+1; // find Delimiter in String and move pointer
        // Serial.println(tmpValues[i]);
    }

    values = *tmpValues;
    return true;

}
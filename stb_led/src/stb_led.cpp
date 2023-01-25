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
        switch (TimeVars[i].lightMode) {
            case ledCmds::setRunning: running(i); break;
            case ledCmds::setRunningPWM: runningPWM(); break;
            case ledCmds::setBlinking: blinking(i); break;
            case ledCmds::setfade2color: fade2color(i); break;
        }
    //delay(50);
    }
    lightDog();
}




/**
 * @brief runningLight
 * 
 * @param stripNo 
 */
void STB_LED::running(int stripNo) {
    
    if (TimeVars[stripNo].modeDuration == 0){    // first run after call
        TimeVars[stripNo].deltaTime =  long(round(long(TimeVars[stripNo].effektTime) / long(TimeVars[stripNo].usedLED)));
        TimeVars[stripNo].modeDuration  = millis();
    } 

    if (TimeVars[stripNo].modeDuration > millis()) { return; }   // return if modeDuration is not reached
	
    if (TimeVars[stripNo].LED_ON == uint16_t(TimeVars[stripNo].usedLED) && TimeVars[stripNo].pauseTime > 0) { // condition if LED_ON reaches end of actLED if pause is active
        TimeVars[stripNo].modeDuration  = TimeVars[stripNo].modeDuration  + TimeVars[stripNo].pauseTime;  // reset timing with defined pauseTime
        setLEDToClr(stripNo,TimeVars[stripNo].usedLED - 1, clrBlack);                                   // turn off Last LED 
        TimeVars[stripNo].LED_ON += 1;                                                                  // increaese counter to avoid pause condition on next Loop   
        return;   
    }

    TimeVars[stripNo].modeDuration  += TimeVars[stripNo].deltaTime;                 // set new timepoint

    if (TimeVars[stripNo].LED_ON == 0){
        setLEDToClr(stripNo,TimeVars[stripNo].usedLED - 1, clrBlack);             // turn off LED when reset
    } else {
        setLEDToClr(stripNo,TimeVars[stripNo].LED_ON - 1, clrBlack);             // turn off LED 
    }
    
    if (TimeVars[stripNo].LED_ON > uint16_t(TimeVars[stripNo].usedLED) - 1) {    // condition if LED_ON reaches end of actPWM
            TimeVars[stripNo].LED_ON = 0;                                       // reset LED-Counter
    }                    
    setLEDToClr(stripNo,TimeVars[stripNo].LED_ON, TimeVars[stripNo].color[0]);            // turn on new LED  

    TimeVars[stripNo].LED_ON += 1;                                  // set new for next loop LED 


}


/**
 * @brief runningPWM
 * currently only supports start with PWM 0
 */
void STB_LED::runningPWM() { //(CW 281022)

    
    
    if (TimeVars[0].modeDuration == 0){// first run after call
            TimeVars[0].deltaTime =  long(round(long(TimeVars[0].effektTime) / long(TimeVars[0].usedLED)));
            TimeVars[0].modeDuration  = millis();
    } 

    if (TimeVars[0].modeDuration > millis()) { return; }       // return if modeDuration is not reached
	
    if (TimeVars[0].LED_ON == uint16_t(TimeVars[0].usedLED) && TimeVars[0].pauseTime > 0) { // condition if LED_ON reaches end of actPWM if pause is active
        TimeVars[0].modeDuration  = TimeVars[0].modeDuration  + TimeVars[0].pauseTime;  // reset timing with defined pauseTime
        setStripToClr(TimeVars[0].usedLED - 1, clrBlack);             // turn off Last LED 
        TimeVars[0].LED_ON += 1;                                 // increaese counter to avoid pause condition new LED   
        return;   
    }

    TimeVars[0].modeDuration  += TimeVars[0].deltaTime;                 // set new timepoint

    if (TimeVars[0].LED_ON == 0){
        setStripToClr(TimeVars[0].usedLED - 1, clrBlack);             // turn off LED when reset
    }
    else{
        setStripToClr(TimeVars[0].LED_ON - 1, clrBlack);             // turn off LED 
    }
    
    if (TimeVars[0].LED_ON > uint16_t(TimeVars[0].usedLED) - 1) { // condition if LED_ON reaches end of actPWM
            TimeVars[0].LED_ON = 0; // reset LED-Counter
    }                    
    setStripToClr(TimeVars[0].LED_ON, TimeVars[0].color[0]);            // turn on new LED  

    TimeVars[0].LED_ON += 1;                                  // set new for next loop LED 
}


/**
 * @brief WIP @Christian walter
 * @param stripNo 
*/
void STB_LED::blinking(int stripNo) { //(CW 281022)
  
    if (TimeVars[stripNo].modeDuration > millis()) { return; } 
    
    if(TimeVars[stripNo].LED_ON == 0){
        TimeVars[stripNo].modeDuration  += TimeVars[stripNo].blinkTime[0];
        TimeVars[stripNo].LED_ON = 1;
        setStripToClr(stripNo, TimeVars[stripNo].color[0]);             // turn off LED
    }
    else{
        TimeVars[stripNo].modeDuration += TimeVars[stripNo].blinkTime[1];
        TimeVars[stripNo].LED_ON = 0;
        setStripToClr(stripNo, TimeVars[stripNo].color[1]);
    }
}


/**
 * @brief 
 * 
 * 
 * @param stripNo 
 */
void STB_LED::fade2color(int stripNo){    

    
    // flinkering
  /*  int possibility = rand() % 101; 
        if (possibility > 5){                
            setStripToClr(stripNo, clrBlack);    
            delay(5);              
        }
 */
    unsigned long actTime = millis();

    if (TimeVars[stripNo].modeDuration  < actTime) { // last call force color
        TimeVars[stripNo].lightMode = -1;          
        setStripToClr(stripNo, TimeVars[stripNo].color[1]);    
        return; 
    } 

    uint8_t clr1_1 = TimeVars[stripNo].color[0] >> 16; //bit shift 16 (Adafruit_NeoPixel::color)
    uint8_t clr1_2 = TimeVars[stripNo].color[0]; //bit shift 8
    uint8_t clr1_3 = TimeVars[stripNo].color[0] >> 8; //bit shift 0

    uint8_t clr2_1 = TimeVars[stripNo].color[1] >> 16; //bit shift 16 (Adafruit_NeoPixel::color)
    uint8_t clr2_2 = TimeVars[stripNo].color[1]; //bit shift 8
    uint8_t clr2_3 = TimeVars[stripNo].color[1] >> 8; //bit shift 0
    

    // linear interpolation of new color    
    unsigned long t0 = TimeVars[stripNo].modeDuration - TimeVars[stripNo].effektTime;
    unsigned long t1 = TimeVars[stripNo].modeDuration;

    uint8_t newclr_1 = clr1_1*(t1 - actTime)/TimeVars[stripNo].effektTime + clr2_1*( actTime - t0)/TimeVars[stripNo].effektTime;
    uint8_t newclr_2 = clr1_2*(t1 - actTime)/TimeVars[stripNo].effektTime + clr2_2*( actTime - t0)/TimeVars[stripNo].effektTime;
    uint8_t newclr_3 = clr1_3*(t1 - actTime)/TimeVars[stripNo].effektTime + clr2_3*( actTime - t0)/TimeVars[stripNo].effektTime;
    
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
    int stripNo;
    int stripArray[4] = {-1,-1,-1,-1};
    //for (int i=0; i < STRIPE_CNT; i++) {lightMode[i] = -1;}

    switch (cmdNo) {
        case setAll:
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            setAllStripsToClr(setClr);
            for (int stripNo=0; stripNo < STRIPE_CNT; stripNo++) {
                TimeVars[stripNo].color[0] =  setClr;
                TimeVars[stripNo].lightMode = -1;
            }

        break;
        case ledCmds::setStripToClr:
            if (sscanf(Brain.STB_.rcvdPtr, "%d", &value) <= 0) { return false; } 
            Brain.STB_.rcvdPtr += 2; // only works for single digits of pixels
            if (!getClrsFromBuffer(Brain, setClr)) { return false; }
            // && i< pixelNo
            setStripToClr(value, setClr);
            // @todo safety!
            TimeVars[value].color[0] =  setClr;
            TimeVars[value].lightMode = -1;
        break;

        case setRunning: // starts the runningLight sequence 
            getBufferValues(Brain, 1, stripValue);   
            switch(stripValue){ // combinded blinking has to be added
                case 1: stripNo = 0; break;
                case 2: stripNo = 1; break;
                case 4: stripNo = 2; break;
                case 8: stripNo = 3; break;
            }    
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            getBufferValues(Brain, 1, TimeVars[stripNo].effektTime);
            getBufferValues(Brain, 1, TimeVars[stripNo].usedLED);
            getBufferValues(Brain, 1, TimeVars[stripNo].pauseTime);
            Brain.sendAck();
            setStripToClr(stripNo, clrBlack);
            TimeVars[stripNo].color[0] =  STB_LED::Strips->Color(clrs[0], clrs[1], clrs[2]); 
            if  (TimeVars[stripNo].lightMode != ledCmds::setRunning){      // for restart at the same LED (looks better)
                TimeVars[stripNo].LED_ON = 0;
            }
            TimeVars[stripNo].lightMode = ledCmds::setRunning;  //for PWM only the first state is necessary  
            TimeVars[stripNo].modeDuration = 0; // reset Timer
        break;

        case setRunningPWM: // starts the runningLight sequence (CW 281022)
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            getBufferValues(Brain, 1, TimeVars[0].effektTime);
            getBufferValues(Brain, 1, TimeVars[0].usedLED);
            getBufferValues(Brain, 1, TimeVars[0].pauseTime);
            Brain.sendAck();
            setStripToClr(stripNo, clrBlack);
            TimeVars[stripNo].color[0] =  STB_LED::Strips->Color(clrs[0], clrs[1], clrs[2]); 
            if  (TimeVars[0].lightMode != ledCmds::setRunningPWM){ // for restart at the same LED (looks better)
                TimeVars[0].LED_ON = 0;
            }
            TimeVars[0].lightMode = ledCmds::setRunningPWM;  //for PWM only the first state is necessary  
            TimeVars[0].deltaTime =  long(round(long(TimeVars[0].effektTime) / long(TimeVars[0].usedLED)));
            TimeVars[0].modeDuration  = millis();
            
        break;
        
        case setBlinking:  // starts the blinking sequence
            getBufferValues(Brain, 1, stripValue);   
            switch(stripValue){ // combinded blinking has to be added
                case 1: stripArray[0] = 0; break;
                case 2: stripArray[0] = 1; break;
                case 3: stripArray[0] = 0; stripArray[1] = 1; break;
                case 4: stripArray[0] = 2; break;
                case 7: stripArray[0] = 0; stripArray[1] = 1; stripArray[2] = 2; break;
                case 8: stripArray[0] = 3; break;
                case 15: stripArray[0] = 0; stripArray[1] = 1; stripArray[2] = 2; stripArray[3] = 3;   break;
            }

            stripNo = stripArray[0];

            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            TimeVars[stripNo].color[0] =  STB_LED::Strips->Color(clrs[0], clrs[1], clrs[2]);  
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            TimeVars[stripNo].color[1] =  STB_LED::Strips->Color(clrs[0], clrs[1], clrs[2]);  
            getBufferValues(Brain, 1, TimeVars[stripNo].blinkTime[0]); getBufferValues(Brain, 1, TimeVars[stripNo].blinkTime[1]);   
            Brain.sendAck();

            TimeVars[stripNo].lightMode = setBlinking; 
            TimeVars[stripNo].modeDuration  = millis();
            blinking(stripNo);
            
            for (int i=1; i<STRIPE_CNT; i++) { // for setting 
                if (stripArray[i] > -1){
                    stripNo = stripArray[i];
                    TimeVars[stripNo].color[0] =  TimeVars[stripArray[0]].color[0];  
                    TimeVars[stripNo].color[1] =  TimeVars[stripArray[0]].color[1];  
                    TimeVars[stripNo].blinkTime[0] = TimeVars[stripArray[0]].blinkTime[0];
                    TimeVars[stripNo].blinkTime[1] = TimeVars[stripArray[0]].blinkTime[1];
                    TimeVars[stripNo].lightMode = setBlinking; 
                    TimeVars[stripNo].modeDuration  = millis();       
                    blinking(stripNo);
                }
            }
        break;

        case setfade2color: // fading to new color

            getBufferValues(Brain, 1, stripValue);   
               
            switch(stripValue){ // combinded blinking has to be added
                case 1: stripArray[0] = 0; break;
                case 2: stripArray[0] = 1; break;
                case 3: stripArray[0] = 0; stripArray[1] = 1; break;
                case 4: stripArray[0] = 2; break;
                case 7: stripArray[0] = 0; stripArray[1] = 1; stripArray[2] = 2; break;
                case 8: stripArray[0] = 3; break;
                case 15: stripArray[0] = 0; stripArray[1] = 1; stripArray[2] = 2; stripArray[3] = 3;   break;
            }

            stripNo = stripArray[0];
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            TimeVars[stripNo].color[0] =  STB_LED::Strips->Color(clrs[0], clrs[1], clrs[2]);  
            getBufferValues(Brain, 1, clrs[0]); getBufferValues(Brain, 1, clrs[1]); getBufferValues(Brain, 1, clrs[2]);
            TimeVars[stripNo].color[1] =  STB_LED::Strips->Color(clrs[0], clrs[1], clrs[2]);  
            getBufferValues(Brain, 1, TimeVars[stripNo].effektTime);  
            Brain.sendAck();
            TimeVars[stripNo].lightMode = ledCmds::setfade2color; 
            TimeVars[stripNo].modeDuration  = millis() + TimeVars[stripNo].effektTime;       
            fade2color(stripNo);

            for (int i=1; i<STRIPE_CNT; i++) { // for setting of more than one PWM
                if (stripArray[i] > -1){
                    stripNo = stripArray[i];
                    TimeVars[stripNo].color[0] =  TimeVars[stripArray[0]].color[0];  
                    TimeVars[stripNo].color[1] =  TimeVars[stripArray[0]].color[1];  
                    TimeVars[stripNo].effektTime = TimeVars[stripArray[0]].effektTime;  
                    TimeVars[stripNo].lightMode = ledCmds::setfade2color; 
                    TimeVars[stripNo].modeDuration  = millis() + TimeVars[stripNo].effektTime;       
                    fade2color(stripNo);
                }
            }
            
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
    setClr = Strips[0].Color(clrs[0], clrs[1], clrs[2]);
    // everything needed has been read, hence answering the mother can be done here already
    Brain.sendAck();
    return true;
}

/**
 * @brief 
 * 
 * @param Brain 
 * @param StripNo 
 * @param VarNo 
 * @return true 
 * @return false 
 */
/* bool STB_LED::writeBuffer2Matrix(STB_BRAIN &Brain, int StripNo,  int VarNo){
    
    int tmpValues[5];
    
    for (int i=0; i <=nrValues-1;i++){
        sscanf(Brain.STB_.rcvdPtr,"%d", &tmpValues[i]); //gets Value No i from Buffer  
        Brain.STB_.rcvdPtr=strstr( Brain.STB_.rcvdPtr, "_")+1; // find Delimiter in String and move pointer
        // Serial.println(tmpValues[i]);
    }

    values = *tmpValues;
    return true;

} */

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

bool STB_LED::lightDog(){
    // constantly reset the light on actual state. Applied to compensate light faults because of current fluctuations
    for (int stripNo = 0; stripNo < STRIPE_CNT; stripNo++) {
        if (TimeVars[stripNo].lightMode < 0) { // only steady light
            setStripToClr(stripNo, TimeVars[stripNo].color[0] );
            //setStripToClr(stripNo,  STB_LED::Strips->Color(123, 32, 0));
        }
    //delay(50);
    }

    return true;
}
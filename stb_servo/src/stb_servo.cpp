/**
 * @file stb_servo.cpp
 * @author Christian Walter (pauseandplay@cybwalt.de)
 * @brief 
 * @version 0.1
 * @date 2023-02-16
 * 
 */
#include "stb_servo.h"


STB_SERVO::STB_SERVO() {};

/**
 * @brief initializes the parsed RFID reader not working
 * @param reader 
 * @return success
 */
bool STB_SERVO::ServoInit() { // 

    Serial.print(F("\nServo init ... \n"));

    for( int i = 0; i < SERVO_CNT; i++ ){
        //Servo_i.attach(ServoPins[i])
       
    }

    return true;
}

void STB_SERVO::moveSingleServo(int stripValue, int pos) {  
    Servo myservo;
    myservo.attach(ServoPins[stripValue]);  // attaches the servo on pin 9 to the servo object
    myservo.write(pos);  
    
    Serial.println("Servo moved!");
    delay(3000);


}

bool STB_SERVO::evaluateCmds(STB_BRAIN &Brain) {
    
    if (strncmp(KeywordsList::ServoKeyword.c_str(), Brain.STB_.rcvdPtr, KeywordsList::ServoKeyword.length()) != 0) {
        return false;
    }
    // Serial.println("received ledKeyword");
    Brain.STB_.rcvdPtr += KeywordsList::ServoKeyword.length();

    //Serial.println(Brain.STB_.rcvdPtr);
    int cmdNo;
    int stripValue;
    int pos;
    if (sscanf(Brain.STB_.rcvdPtr, "%d", &cmdNo) <= 0) {return false; } 
    Brain.STB_.rcvdPtr += 2;
      switch (cmdNo) {
        case moveServo:        
            getBufferValues(Brain, 1, stripValue);  
            getBufferValues(Brain, 1, pos);  
           /*  Serial.println("Call MoveServo ");
            Serial.println(stripValue);
            Serial.println(pos); */
            Brain.sendAck();
            moveSingleServo(stripValue,pos);
        break;
      }
    return true;

}



bool STB_SERVO::getBufferValues(STB_BRAIN &Brain, int nrValues,  int &values){
    
    int tmpValues[5];
    
    for (int i=0; i <=nrValues-1;i++){
        sscanf(Brain.STB_.rcvdPtr,"%d", &tmpValues[i]); //gets Value No i from Buffer  
        Brain.STB_.rcvdPtr=strstr( Brain.STB_.rcvdPtr, "_")+1; // find Delimiter in String and move pointer
        // Serial.println(tmpValues[i]);
    }

    values = *tmpValues;
    return true;

}

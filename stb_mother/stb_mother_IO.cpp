/**
 * @file stb_mother_IO.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief  
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
*/
#include "stb_mother_IO.h"


/**
 * @brief  read all declared inputs as a binary added number
 * @return int 
*/
int STB_MOTHER_IO::getInputs() {
    int value = 0;
    for (int i=0; i<_inputCnt; i++) {
        int inputPin = _inputs[i];
        int inputValue = (1 << inputPin);
        if (!ioPcf.digitalRead((uint8_t) inputPin)) {
            value += inputValue;
        }
    }
    return value;
}


/**
 * @brief Construct a new stb mother io::ioinit object 
 * @todo could maybe take an address... or make another general pcf lib?
 * @param inputs 
 * @param inputCnt 
 * @param outputs 
 * @param outputCnt 
 * @return true 
*/
bool STB_MOTHER_IO::ioInit(int inputs[], int inputCnt, int outputs[], int outputCnt) {

    _inputCnt = inputCnt;
    _outputCnt = outputCnt;
    memcpy(_outputs, outputs, outputCnt);

    ioPcf.begin(RESET_I2C_ADD);
    for (int i=0; i<inputCnt; i++) {
        ioPcf.pinMode((uint8_t) inputs[i], INPUT);
        ioPcf.digitalWrite((uint8_t) inputs[i], HIGH);
    }
    for (int i=0; i<outputCnt; i++) {
        ioPcf.pinMode((uint8_t) outputs[i], OUTPUT);
        ioPcf.digitalWrite((uint8_t) outputs[i], HIGH);
    }
    return true;
}

/**
 * @brief  resets the outs to HIGH
*/
void STB_MOTHER_IO::outputReset() {
    for (int i=0; i<_outputCnt; i++) {
        ioPcf.digitalWrite((uint8_t) _outputs[i], HIGH);
    }
}

/**
 * @brief  
 * @param value 
 * @return success
 * @todo consider safety with false return result, and stored pins/pincnt
*/
bool STB_MOTHER_IO::setOuput(int value) {
    for (int index=0; index<_outputCnt; index++) {
        int pin = _outputs[index];
        int pinValue = 1 << pin;
        ioPcf.digitalWrite((uint8_t) pin, ((pinValue & value) == 0));
    }
    return true;
}

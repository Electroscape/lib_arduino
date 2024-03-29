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
 * @brief  
 * @param pinValueBasedOnIndex 
 * @return int 
*/
int STB_MOTHER_IO::_getInput(bool pinValueBasedOnIndex) {
    int value = 0;
    for (int i=0; i<_inputCnt; i++) {
        int inputPin = _inputs[i];
        int inputValue;
        if (pinValueBasedOnIndex) {
            inputValue = (1 << i);
        } else {
            inputValue = (1 << inputPin);
        }
        if (!ioPcf.digitalRead((uint8_t) inputPin)) {
            value += inputValue;
        }
    }
    return value;
}


/**
 * @brief  read all declared inputs as a binary added number
 * @param pinValueBasedOnIndex defaults to false and reads pins based pcf order nor input declaration index
 * @return int 
*/
int STB_MOTHER_IO::getInputs(bool pinValueBasedOnIndex) {
    int ret = _getInput(pinValueBasedOnIndex);
    delay(2);
    if (ret == _getInput(pinValueBasedOnIndex)) {
        return ret;
    } else {
        return 0;
    }
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
bool STB_MOTHER_IO::ioInit(int inputs[], size_t inputCnt, int outputs[], size_t outputCnt) {

    _inputCnt = inputCnt / sizeof(int);
    _outputCnt = outputCnt / sizeof(int);
    memcpy(_outputs, outputs, outputCnt);
    memcpy(_inputs, inputs, inputCnt);

    ioPcf.begin(RESET_I2C_ADD);
    for (int i=0; i<_inputCnt; i++) {
        ioPcf.pinMode((uint8_t) inputs[i], INPUT);
        ioPcf.digitalWrite((uint8_t) inputs[i], HIGH);
    }
    for (int i=0; i<_outputCnt; i++) {
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
bool STB_MOTHER_IO::setOuput(int value, bool pinValueBasedOnIndex) {
    int pinValue;
    int pin;
    for (int index=0; index<_outputCnt; index++) {
        pin = _outputs[index];
        if (pinValueBasedOnIndex) {
            pinValue = 1 << index;
        } else {
            pinValue = 1 << pin;
        }
        ioPcf.digitalWrite((uint8_t) pin, ((pinValue & value) == 0));
    }
    return true;
}

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
    for (int i=0; i<PCF_PIN_CNT; i++) {
        int inputPin = _inputs[i];
        int inputValue;
        if (pinValueBasedOnIndex) {
            inputValue = (1 << i);
        } else {
            inputValue = (1 << inputPin);
        }
        if (!io_in_pcf.digitalRead((uint8_t) inputPin)) {
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
 * @brief sets up the IO pcfs used by the class 

*/
void STB_MOTHER_IO::ioInit() {

    io_in_pcf.begin(IO_IN_ADD);
    io_out_pcf.begin(IO_OUT_ADD);

    for (int i=0; i<PCF_PIN_CNT; i++) {
        io_in_pcf.pinMode((uint8_t) i, INPUT);
        io_in_pcf.digitalWrite((uint8_t) i, HIGH);
        io_out_pcf.pinMode((uint8_t) i, OUTPUT);
        io_out_pcf.digitalWrite((uint8_t) i, HIGH);
    }
}


/**
 * @brief  resets the outs to HIGH
*/
void STB_MOTHER_IO::outputReset() {
    for (int i=0; i<PCF_PIN_CNT; i++) {
        io_out_pcf.digitalWrite((uint8_t) _outputs[i], HIGH);
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
    for (int index=0; index<PCF_PIN_CNT; index++) {
        pin = _outputs[index];
        if (pinValueBasedOnIndex) {
            pinValue = 1 << index;
        } else {
            pinValue = 1 << pin;
        }
        io_out_pcf.digitalWrite((uint8_t) pin, ((pinValue & value) == 0));
    }
    return true;
}

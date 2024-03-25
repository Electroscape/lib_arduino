#pragma once

#include <PCF8574.h>
#include <stb_mother.h>


class STB_MOTHER_IO {
    private:
    PCF8574 io_in_pcf;
    PCF8574 io_out_pcf;
    int _inputs[PCF_PIN_CNT] = {0};
    int _outputs[PCF_PIN_CNT] = {0};
    int _getInput(bool pinValueBasedOnIndex);
    public:
    int getInputs(bool pinValueBasedOnIndex=false);
    void ioInit();
    void outputReset();
    // technicall we should make the bool also for getInputs...
    bool setOuput(int value, bool pinValueBasedOnIndex=false);
};
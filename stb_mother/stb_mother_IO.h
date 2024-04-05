#pragma once

#include <PCF8574.h>
#include <stb_mother.h>


class STB_MOTHER_IO {
    private:
    PCF8574 io_in_pcf;
    PCF8574 io_out_pcf;
    int _getInput();
    public:
    int getInputs();
    void ioInit();
    void outputReset();
    // technicall we should make the bool also for getInputs...
    void setOuput(int value);
};
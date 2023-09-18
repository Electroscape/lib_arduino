#pragma once

#include <PCF8574.h>
#include <stb_mother.h>


class STB_MOTHER_IO {
    private:
    PCF8574 ioPcf;
    int _inputCnt;
    int _inputs[8] = {0};
    int _outputs[8] = {0};
    int _outputCnt;
    int _getInput(bool pinValueBasedOnIndex);
    public:
    int getInputs(bool pinValueBasedOnIndex=false);
    bool ioInit(int inputs[], size_t inputCnt, int outputs[], size_t outputCnt);
    void outputReset();
    // technicall we should make the bool also for getInputs...
    bool setOuput(int value, bool pinValueBasedOnIndex=false);
};
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
    public:
    int getInputs();
    bool ioInit(int inputs[], int inputCnt, int outputs[], int outputCnt);
    void outputReset();
    bool setOuput(int value);
};
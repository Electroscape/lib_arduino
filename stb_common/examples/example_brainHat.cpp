#include <stb_common.h>

STB STB;

void setup() {
    STB.begin();

    STB.rs485SetSlaveAddr(0);
    
    STB.i2cScanner();
    STB.printSetupEnd(); 
}

void loop() {
    STB.rs485SlaveRespond();
    while (STB.rcvdPtr != NULL) {
        if (strncmp((char *) ledKeyword, STB.rcvdPtr, 4) == 0) {
            // do stuff
        }
    }
}
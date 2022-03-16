#pragma once

#include <Arduino.h>
#include <PCF8574.h> /* I2C Port Expander  */
#include <Wire.h>    /* I2C library */

// Defined by hardware
#define MAX_CTRL_PIN A0
#define MAX485_WRITE HIGH
#define MAX485_READ LOW

#ifndef RELAY_I2C_ADD
    #define RELAY_I2C_ADD 0x3F
#endif 

class STB {
    private:
    static bool serial_init();
    static void print_info();
    
    public:
    static void begin();
    static void printWithHeader(String message, String source=String("SYS"));
    static void print_setup_end();

    static bool i2c_scanner();
    static void software_reset();
    
    static bool relay_init(Expander_PCF8574 relay, int pins[], int initvals[], int amount=8);
};





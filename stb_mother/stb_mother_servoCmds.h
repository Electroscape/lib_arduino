#pragma once
#include <stb_servo_shared.h>
#include <stb_mother.h>

class SERVO_CMDS {
    
    private:
    // static void clrToString(char *&addToMsg, const int clr[3], int brightness);
    
    public:

    /*could possibly just pass the Mother class once? 
    brainNo as optional param? otherwise take the currently polled slave
    */
    static void moveServo(STB_MOTHER &Mother, int brainNo, int stripNo, int pos);
};
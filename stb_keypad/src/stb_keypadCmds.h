/**
 * @file stb_keypadCmds.h
 * @author Martin Pek (martin.pek@web.de)
 * @brief commands shared by both brain and mother
 * @version 0.1
 * @date 2022-09-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <Arduino.h>

const String keypadCmd = "!KeyCmd";

enum KeypadCmds {
    post,
    evaluate,
    correct,
    wrong,
    reset,
    amountOfCmds = 5
};


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
#include <stb_shared.h>

static const String keypadCmdKeyword;

enum KeypadCmds {
    post,
    evaluate,
    correct,
    wrong,
    reset,
    amountOfCmds = 5
};


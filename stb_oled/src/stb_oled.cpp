/**
 * @file stb_oled.cpp
 * @author Martin Pek (martin.pek@web.de)
 * @brief 
 * @version 0.1
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "stb_oled.h"

/**
 * @brief starts the oleds
 * @param oled 
 * @param oledType 
 * @param oledAddr 
 * @return true 
 */
bool STB_OLED::oledInit(SSD1306AsciiWire *oled, DevType oledType, int oledAddr) {
    oled->begin(&SH1106_128x64, oledAddr);
    // oledset400kHz();
    // oledsetScroll(true);
    // oledsetFont(Adafruit5x7);
    // oledprintln(" fucking work already ");
    // STB_OLED::startupScreen(oled);
    return true;
}

/**
 * @brief shows proccess during system startup
 * 
 * @param oled 
 */
void STB_OLED::startupScreen(SSD1306AsciiWire &oled) {
    oled.clear();
    oled.setFont(Verdana12_bold);
    oled.println("\n System startup...");
}
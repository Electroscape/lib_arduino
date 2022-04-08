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
    oled->set400kHz();
    oled->setScroll(true);
    oled->setFont(Verdana12_bold);
    oled->clear();
    // oled->println("  System startup...");
    return true;
}
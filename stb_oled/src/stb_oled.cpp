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


void STB_OLED::clearAbove(SSD1306AsciiWire oled, uint8_t row) {
    // what is an inline fnc?
    uint8_t i = row;
    while (i < oled.displayRows()) {
        oled.setRow(i);
        oled.clearToEOL();
        i++;
    }
    oled.setRow(row);
    oled.setCol((uint8_t) 3);
    delay(5);
}


/**
 * @brief starts the oleds
 * @param oled 
 * @param oledType 
 * @param oledAddr 
 * @return true 
 */
bool STB_OLED::oledInit(SSD1306AsciiWire *oled, DevType oledType, int oledAddr) {
    Serial.print(F("\n\nOLED init on Address: 0x"));
    Serial.println(oledAddr, HEX);
    delay(10);
    oled->begin(&SH1106_128x64, oledAddr);
    // oled->set400kHz();
    oled->setScroll(true);
    oled->setFont(Verdana12_bold);
    oled->clear();
    // oled->println("  System startup...");
    Serial.println(F("Successful"));
    delay(10);
    return true;
}


/**
 * @brief sets oled to the default font define here
 * @param oled 
 */
void STB_OLED::setDefaultFont(SSD1306AsciiWire *oled) {
    oled->setFont(Adafruit5x7);
    // Verdana12_bold
}


/**
 * @brief prints to Serial and Oled
 * @param oled 
 * @param text 
 */
void STB_OLED::systemPrint(SSD1306AsciiWire oled, String text) {
    oled.print(text);
    Serial.print(text);
}


/**
 * @brief prints to Serial and Oled with newline in the end
 * @param oled 
 * @param text 
 */
void STB_OLED::systemPrintln(SSD1306AsciiWire oled, String text) {
    oled.println(text);
    Serial.println(text);
}


/**
 * @brief clears Oled and writes headline and move to next writeable line
 * @param oled 
 * @param text 
 */
void STB_OLED::writeHeadline(SSD1306AsciiWire oled, String text) {
    oled.clear();
    oled.setFont(Verdana12_bold);
    oled.println(text);
    setDefaultFont(&oled);
    // if set to 1 it collides with the smaller followup font
    oled.setRow((uint8_t) 2);
}
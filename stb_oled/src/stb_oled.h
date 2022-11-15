#pragma once
#include "SSD1306AsciiWire.h"
// due to casing we loose the first row hence shifting everything downwards
#define rowOffset 1

class STB_OLED {
    private:
    public:
    static void clearAbove(SSD1306AsciiWire oled, uint8_t row);
    static bool oledInit(SSD1306AsciiWire *oled, DevType oledType, int oledAddr=0x3C);
    static void setDefaultFont(SSD1306AsciiWire *oled);
    static void systemPrint(SSD1306AsciiWire oled, String text);
    static void systemPrintln(SSD1306AsciiWire oled, String text);
    static void writeCenteredLine(SSD1306AsciiWire *oled, String text);
    static void writeHeadline(SSD1306AsciiWire *oled, String text);
    static void writeToLine(SSD1306AsciiWire *oled, uint8_t row, String text, bool centered=false);
};

#include "SSD1306AsciiWire.h"

class STB_OLED {
    private:
    static void startupScreen(SSD1306AsciiWire &oled);
    public:
    static bool oledInit(SSD1306AsciiWire oled, DevType oledType, int oledAddr=0x3C);
};

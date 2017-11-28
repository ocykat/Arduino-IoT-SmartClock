/* Tutorial: http://overskill.alexshu.com/128x64-12864-lcd-w-arduino-serial/ */

#include <U8glib.h>

U8GLIB_ST7920_128X64_4X lcd(10);

void draw() {
    lcd.setFont(u8g_font_unifont);
    lcd.drawStr(0, 22, "Hello World");
}

void setup() {
    if (lcd.getMode() == U8G_MODE_R3G3B2) {
        lcd.setColorIndex(255);
    }
    else if (lcd.getMode() == U8G_MODE_GRAY2BIT) {
        lcd.setColorIndex(3);
    }
    else if (lcd.getMode() == U8G_MODE_BW) {
        lcd.setColorIndex(1);
    }
    else if (lcd.getMode() == U8G_MODE_HICOLOR) {
        lcd.setHiColorByRGB(255, 255, 255);
    }
}

void loop() {
    lcd.firstPage();
    do {
        draw();
    } while(lcd.nextPage());
    delay(50);
}

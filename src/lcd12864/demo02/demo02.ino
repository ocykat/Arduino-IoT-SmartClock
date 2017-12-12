/* Tutorial: http://overskill.alexshu.com/128x64-12864-lcd-w-arduino-serial/ */

#include <U8glib.h>

U8GLIB_ST7920_128X64_4X lcd(53);

int frameCnt = 0;

char strarr[4][12] = {
    "One",
    "Two",
    "Three",
    "Four"
};

void draw(int cnt) {
    lcd.setFont(u8g_font_unifont);
    lcd.setFontPosBottom();
    Serial.println("Draw..");
    lcd.drawStr(cnt * 2, (10 + cnt * 4), strarr[cnt]);
}

void setup() {
    Serial.begin(9600);
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
    delay(4000);
}

void loop() {
    frameCnt = (frameCnt + 1) % 4;
    bool redraw = true;
    if (redraw) {
        lcd.firstPage();
        do {
            draw(frameCnt);
        }
        while (lcd.nextPage());
        redraw = false;
    }
    Serial.println(".");
    delay(5000);
}


#include "SED1530_LCD.h"
#include "example_bitmaps.h"

// LCD PINS
uint8_t lcdA0 = 26;
uint8_t lcdRW = 27;
uint8_t lcdEnable = 13;
uint8_t lcdDataPins[] = {19,23,18,5,17,16,4,0};

SED1530_LCD lcd = SED1530_LCD(lcdA0, lcdRW, lcdEnable, lcdDataPins);

void lcd_setup() {
    delay(1000);

    lcd.clearScreen(GLCD_COLOR_CLEAR);
    lcd.updateWholeScreen();
}

void lcd_test() {
    lcd.fillScreen(GLCD_COLOR_CLEAR);
    lcd.drawRect(2, 2, 50, 20, GLCD_COLOR_SET);
    lcd.updateWholeScreen();

    delay(1000);
    // lcd.fillScreen(GLCD_COLOR_CLEAR);
    // delay(1000);

    lcd.fillScreen(GLCD_COLOR_CLEAR);
    lcd.drawCircle(20, 20, 10, GLCD_COLOR_SET);
    lcd.updateWholeScreen();

    delay(1000);

    // for (int l = 0; l <= 3; l++) {
    for (byte i = 1; i <= 6; i++) {
        lcd.setMarker(i, true);
        delay(400);
        lcd.setMarker(i, false);
        delay(100);
        lcd.updateWholeScreen();
    }
    // }
    

    lcd.fillScreen(GLCD_COLOR_CLEAR);

    for (int y = 0; y < 48; y++) {
        for (int x = 0; x < 100; x++) {
            lcd.drawPixel(x, y, GLCD_COLOR_SET);
            lcd.updateWholeScreen();
        }
    }
    
    delay(1000);

    lcd.fillScreen(GLCD_COLOR_CLEAR);
    lcd.cp437(true);   // Use correct CP437 character codes
    lcd.print("Scho"); // Print the plain ASCII first part
    lcd.write(0x94);   // Print the o-with-umlauts
    lcd.println("n");  // Print the last part
    lcd.updateWholeScreen();

    delay(1000);

    lcd.fillScreen(GLCD_COLOR_CLEAR);
    lcd.setMarker(1, GLCD_COLOR_SET);
    lcd.drawBitmap(0, 0, epd_bitmap_Bitmap_binary, 100, 48, GLCD_COLOR_SET);
    lcd.updateWholeScreen();

    delay(10000);

    lcd.fillScreen(GLCD_COLOR_CLEAR);
    lcd.setMarker(2, GLCD_COLOR_SET);
    lcd.drawBitmap(0, 0, epd_bitmap_Bitmap_bayer, 100, 48, GLCD_COLOR_SET);
    lcd.updateWholeScreen();

    delay(10000);

    lcd.fillScreen(GLCD_COLOR_CLEAR);
    lcd.setMarker(3, GLCD_COLOR_SET);
    lcd.drawBitmap(0, 0, epd_bitmap_Bitmap_floyd_steinberg, 100, 48, GLCD_COLOR_SET);
    lcd.updateWholeScreen();

    delay(10000);

    lcd.fillScreen(GLCD_COLOR_CLEAR);
    lcd.setMarker(4, GLCD_COLOR_SET);
    lcd.drawBitmap(0, 0, epd_bitmap_Bitmap_atkinson, 100, 48, GLCD_COLOR_SET);
    lcd.updateWholeScreen();

    delay(10000);
}
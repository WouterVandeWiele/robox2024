
#include <mutex>
#include "SED1530_LCD.h"
#include "example_bitmaps.h"
// robox LCD architecture [docs/lcd_overview.excalidraw.png]


// locking primitives to communicate with the task function
SemaphoreHandle_t xSemaphoreScreenUpdate = NULL;    // trigger to the task to update the screen, release this semafore once done (block sequential updateWholeScreen and let them wait untill the draw screen procedure has finished)
std::mutex canvas_mtx;  // do not allow concurrent access to the GFX canvass object

void run_task(void * param);

// implements a quasi identical interface as SED1530_LCD, but is threaded
class LCD_Threaded {
    public:
        LCD_Threaded(uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA): lcd(A0, RW, EN, DATA) {
            lcd.clearScreen(GLCD_COLOR_CLEAR);
            lcd.updateWholeScreen();
        }

        void invertDisplay(bool i){
            this->lcd.invertDisplay(i);
        }

        void clearScreen(uint16_t color) {
            canvas_mtx.lock();
            this->lcd.clearScreen(color);
            canvas_mtx.unlock();
        }

        void fillScreen(uint16_t color) {
            canvas_mtx.lock();
            this->lcd.fillScreen(color);
            canvas_mtx.unlock();
        }

        void setMarker(uint8_t marker, bool on) {
            this->lcd.setMarker(marker, on);
        }

        void updateWholeScreen(void) {
            // canvas_mtx.lock();
            xSemaphoreTake(xSemaphoreScreenUpdate, portMAX_DELAY);
            // canvas_mtx.unlock();
        }

        void drawPixel(int16_t x, int16_t y, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawPixel(x, y, color);
            canvas_mtx.unlock();
        }
        void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.writeFillRect(x, y, w, h, color);
            canvas_mtx.unlock();
        }
        void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.writeFastVLine(x, y, h, color);
            canvas_mtx.unlock();
        }
        void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.writeFastHLine(x, y, w, color);
            canvas_mtx.unlock();
        }
        void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.writeLine(x0, y0, x1, y1, color);
            canvas_mtx.unlock();
        }
        void setRotation(uint8_t r) {
            canvas_mtx.lock();
            this->lcd.setRotation(r);
            canvas_mtx.unlock();
        }
        void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawFastVLine(x, y, h, color);
            canvas_mtx.unlock();
        }
        void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawFastHLine(x, y, w, color);
            canvas_mtx.unlock();
        }
        void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.fillRect(x, y, w, h, color);
            canvas_mtx.unlock();
        }
        void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawLine(x0, y0, x1, y1, color);
            canvas_mtx.unlock();
        }
        void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawRect(x, y, w, h, color);
            canvas_mtx.unlock();
        }

        void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawCircle(x0, y0, r, color);
            canvas_mtx. unlock();
        }
        void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawCircleHelper(x0, y0, r, cornername, color);
            canvas_mtx. unlock();
        }
        void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.fillCircle(x0, y0, r, color);
            canvas_mtx. unlock();
        }
        void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.fillCircleHelper(x0, y0, r, cornername, delta, color);
            canvas_mtx. unlock();
        }
        void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawTriangle(x0, y0, x1, y1, x2, y2, color);
            canvas_mtx. unlock();
        }
        void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.fillTriangle(x0, y0, x1, y1, x2, y2, color);
            canvas_mtx. unlock();
        }
        void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawRoundRect(x0, y0, w, h, radius, color);
            canvas_mtx. unlock();
        }
        void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.fillRoundRect(x0, y0, w, h, radius, color);
            canvas_mtx. unlock();
        }
        void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawBitmap(x, y, bitmap, w, h, color);
            canvas_mtx. unlock();
        }
        void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg) {
            canvas_mtx.lock();
            this->lcd.drawBitmap(x, y, bitmap, w, h, color, bg);
            canvas_mtx. unlock();
        }
        void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawBitmap(x, y, bitmap, w, h, color);
            canvas_mtx. unlock();
        }
        void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) {
            canvas_mtx.lock();
            this->lcd.drawBitmap(x, y, bitmap, w, h, color, bg);
            canvas_mtx. unlock();
        }
        void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) {
            canvas_mtx.lock();
            this->lcd.drawXBitmap(x, y, bitmap, w, h, color);
            canvas_mtx. unlock();
        }
        void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {
            canvas_mtx.lock();
            this->lcd.drawChar(x, y, c, color, bg, size);
            canvas_mtx. unlock();
        }
        void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y) {
            canvas_mtx.lock();
            this->lcd.drawChar(x, y, c, color, bg, size_x, size_y);
            canvas_mtx. unlock();
        }

        void print(const char* s) {
            canvas_mtx.lock();
            this->lcd.print(s);
            canvas_mtx. unlock();
        }
        void write(uint8_t c) {
            canvas_mtx.lock();
            this->lcd.write(c);
            canvas_mtx. unlock();
        }
        void println(const char* s) {
            canvas_mtx.lock();
            this->lcd.println(s);
            canvas_mtx. unlock();
        }

        void cp437(bool x = true) {
            this->lcd.cp437(x);
        }
        void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
            this->lcd.getTextBounds(string, x, y, x1, y1, w, h);
        }
        void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
            this->lcd.getTextBounds(s, x, y, x1, y1, w, h);
        }
        void getTextBounds(const String &str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
            this->lcd.getTextBounds(str, x, y, x1, y1, w, h);
        }
        void setTextSize(uint8_t s) {
            this->lcd.setTextSize(s);
        }
        void setTextSize(uint8_t sx, uint8_t sy) {
            this->lcd.setTextSize(sx, sy);
        }
        void setFont(const GFXfont *f = NULL) {
            this->lcd.setFont(f);
        }

        void setCursor(int16_t x, int16_t y) {
            this->lcd.setCursor(x, y);
        }

        void setTextColor(uint16_t c) {
            this->lcd.setTextColor(c);
        }

        void setTextColor(uint16_t c, uint16_t bg) {
            this->lcd.setTextColor(c, bg);
        }

        void setTextWrap(bool w) {
            this->lcd.setTextWrap(w);
        }

        int16_t width(void) {
            return this->lcd.width();
        }
        int16_t height(void) {
            return this->lcd.height();
        }
        uint8_t getRotation(void) {
            return this->lcd.getRotation();
        }
        int16_t getCursorX(void) {
            return this->lcd.getCursorX();
        }
        int16_t getCursorY(void) {
            return this->lcd.getCursorY();
        }

        SED1530_LCD lcd;
        TaskHandle_t threaded_task;

};

void run_task(void * param) {
    LCD_Threaded* p = (LCD_Threaded*)param;

    Serial.println("thread lcd is running");
    while (true) {
        if (uxSemaphoreGetCount(xSemaphoreScreenUpdate) == 0) {
            Serial.println("updating screen");
            canvas_mtx.lock();
            p->lcd.updateWholeScreen();
            canvas_mtx.unlock();
            xSemaphoreGive(xSemaphoreScreenUpdate);
        }
    }
}

// LCD PINS
uint8_t lcdA0 = 26;
uint8_t lcdRW = 27;
uint8_t lcdEnable = 13;
uint8_t lcdDataPins[] = {19,23,18,5,17,16,4,0};

LCD_Threaded lcd_t(lcdA0, lcdRW, lcdEnable, lcdDataPins);

void lcd_setup() {
    delay(1000);

    xSemaphoreScreenUpdate = xSemaphoreCreateBinary();

    xTaskCreate(
        run_task,       //Function to implement the task 
        "lcd_thread", //Name of the task
        6000,       //Stack size in words 
        (void*)&lcd_t,       //Task input parameter 
        0,          //Priority of the task 
        &(lcd_t.threaded_task)       //Task handle.
    );
}

void lcd_test() {
    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.drawRect(2, 2, 50, 20, GLCD_COLOR_SET);
    lcd_t.updateWholeScreen();

    delay(1000);
    // lcd.fillScreen(GLCD_COLOR_CLEAR);
    // delay(1000);

    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.drawCircle(20, 20, 10, GLCD_COLOR_SET);
    lcd_t.updateWholeScreen();

    delay(1000);

    // for (int l = 0; l <= 3; l++) {
    for (byte i = 1; i <= 6; i++) {
        lcd_t.setMarker(i, true);
        delay(400);
        lcd_t.setMarker(i, false);
        delay(100);
        lcd_t.updateWholeScreen();
    }
    // }
    

    lcd_t.fillScreen(GLCD_COLOR_CLEAR);

    for (int y = 0; y < 48; y++) {
        for (int x = 0; x < 100; x++) {
            lcd_t.drawPixel(x, y, GLCD_COLOR_SET);
            lcd_t.updateWholeScreen();
        }
    }
    
    delay(1000);

    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.cp437(true);   // Use correct CP437 character codes
    lcd_t.print("Scho"); // Print the plain ASCII first part
    lcd_t.write(0x94);   // Print the o-with-umlauts
    lcd_t.println("n");  // Print the last part
    lcd_t.updateWholeScreen();

    delay(1000);

    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.setMarker(1, GLCD_COLOR_SET);
    lcd_t.drawBitmap(0, 0, epd_bitmap_Bitmap_binary, 100, 48, GLCD_COLOR_SET);
    lcd_t.updateWholeScreen();

    delay(10000);

    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.setMarker(2, GLCD_COLOR_SET);
    lcd_t.drawBitmap(0, 0, epd_bitmap_Bitmap_bayer, 100, 48, GLCD_COLOR_SET);
    lcd_t.updateWholeScreen();

    delay(10000);

    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.setMarker(3, GLCD_COLOR_SET);
    lcd_t.drawBitmap(0, 0, epd_bitmap_Bitmap_floyd_steinberg, 100, 48, GLCD_COLOR_SET);
    lcd_t.updateWholeScreen();

    delay(10000);

    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.setMarker(4, GLCD_COLOR_SET);
    lcd_t.drawBitmap(0, 0, epd_bitmap_Bitmap_atkinson, 100, 48, GLCD_COLOR_SET);
    lcd_t.updateWholeScreen();

    delay(10000);
}
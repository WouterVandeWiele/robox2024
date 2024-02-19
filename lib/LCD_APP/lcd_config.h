
#include <mutex>
#include "SED1530_LCD.h"
#include "example_bitmaps.h"

// robox LCD architecture [docs/lcd_overview.excalidraw.png]


// locking primitives to communicate with the task function
SemaphoreHandle_t xSemaphoreScreenUpdate = NULL;    // trigger to the task to update the screen, release this semafore once done (block sequential updateWholeScreen and let them wait untill the draw screen procedure has finished)


class LCD_Threaded: public SED1530_LCD {

    public:
        LCD_Threaded(uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA): SED1530_LCD(A0, RW, EN, DATA) {};

        void updateWholeScreen(void) {
            xSemaphoreTake(xSemaphoreScreenUpdate, 0);
        }

        void taskUpdateWholeScreen(void) {
            SED1530_LCD::updateWholeScreen();
        }

        TaskHandle_t threaded_task;
};


void run_task(void * param) {
    LCD_Threaded* p = (LCD_Threaded*)param;

    while (true) {
        if (uxSemaphoreGetCount(xSemaphoreScreenUpdate) == 0) {
            xSemaphoreGive(xSemaphoreScreenUpdate);
            p->taskUpdateWholeScreen();
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
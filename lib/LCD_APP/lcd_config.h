
#include <utility>  //declarations of unique_ptr
// #include <mutex>
#include "SED1530_LCD.h"
#include "example_bitmaps.h"
#include "general_definitions.h"

// #define RUN_THREADED

// robox LCD architecture [docs/lcd_overview.excalidraw.png]

#if defined RUN_THREADED

// locking primitives to communicate with the task function
SemaphoreHandle_t xSemaphoreScreenUpdate = NULL;    // trigger to the task to update the screen, release this semafore once done (block sequential updateWholeScreen and let them wait untill the draw screen procedure has finished)
static portMUX_TYPE spinlockScreenUpdate = portMUX_INITIALIZER_UNLOCKED;

class LCD_Threaded: public SED1530_LCD {

    public:
        #if defined IO_DIRECT
            LCD_Threaded(uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA): SED1530_LCD(A0, RW, EN, DATA) {};
        #elif defined IO_EXPANDER
            LCD_Threaded(uint8_t address): SED1530_LCD(address) {};
        #endif

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
        taskENTER_CRITICAL(&spinlockScreenUpdate);  // disable interrupts untill we determinded if we need to redraw the LCD

        if (uxSemaphoreGetCount(xSemaphoreScreenUpdate) == 0) {
            xSemaphoreGive(xSemaphoreScreenUpdate);
            taskEXIT_CRITICAL(&spinlockScreenUpdate);

            p->taskUpdateWholeScreen();
        } else {
            taskEXIT_CRITICAL(&spinlockScreenUpdate);
        }
    }
}

#endif

// LCD PINS

#if defined(IO_DIRECT) && defined(RUN_THREADED)
    uint8_t lcdDataPins[] = LCDDATAPINS;

    LCD_Threaded lcd_t(LCDA0, LCDRW, LCDENABLE, lcdDataPins);
#elif defined(IO_EXPANDER) && defined(RUN_THREADED)
    LCD_Threaded lcd_t(IO_EXPANDER_W_ADDRESS);

#elif defined(IO_DIRECT) && !defined(RUN_THREADED)
    uint8_t lcdDataPins[] = LCDDATAPINS;
    SED1530_LCD lcd_t(A0, RW, EN, DATA);

#elif defined(IO_EXPANDER) && !defined(RUN_THREADED)
    SED1530_LCD lcd_t(IO_EXPANDER_W_ADDRESS);
    
#endif

void lcd_setup() {
    ESP_LOGI(LOG_LCD_TAG, "LCD setup");
    lcd_t.lcd_init();
    // delay(1000);

    #if defined RUN_THREADED

    xSemaphoreScreenUpdate = xSemaphoreCreateBinary();

    xTaskCreate(
        run_task,       //Function to implement the task 
        "lcd_thread", //Name of the task
        6000,       //Stack size in words 
        (void*)&lcd_t,       //Task input parameter 
        0,          //Priority of the task 
        &(lcd_t.threaded_task)       //Task handle.
    );

    #endif
}

void lcd_test() {

    /* draw rectangle */
    ESP_LOGI(LOG_LCD_TAG, "LCD loop");
    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.drawRect(2, 2, 50, 20, GLCD_COLOR_SET);
    lcd_t.updateWholeScreen();

    delay(5000);
    // lcd.fillScreen(GLCD_COLOR_CLEAR);
    // delay(1000);

    /* draw circle */
    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.drawCircle(20, 20, 10, GLCD_COLOR_SET);
    lcd_t.updateWholeScreen();

    delay(5000);

    /* rotate through markers */
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

    /* refresh rate test */
    int y = 0;
    // for (int y = 0; y < 48; y++) {
        for (int x = 0; x < 100; x++) {
            lcd_t.drawPixel(x, y, GLCD_COLOR_SET);
            lcd_t.updateWholeScreen();
        }
    // }
    
    delay(5000);

    /* print text test */
    lcd_t.fillScreen(GLCD_COLOR_CLEAR);
    lcd_t.cp437(true);   // Use correct CP437 character codes
    lcd_t.print("Scho"); // Print the plain ASCII first part
    lcd_t.write(0x94);   // Print the o-with-umlauts
    lcd_t.println("n");  // Print the last part
    lcd_t.updateWholeScreen();

    delay(5000);

    /* 4x bitmaps test */

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
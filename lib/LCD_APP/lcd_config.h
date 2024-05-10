
// #include <utility>  //declarations of unique_ptr
// #include <mutex>
#include <GEM_adafruit_gfx.h>
#include <KeyDetector.h>

#include "SED1530_LCD.h"
#include "example_bitmaps.h"
#include "general_definitions.h"

#include "SerialKey.h"

// w (119) -> ↑
// d (100) -> →
// s (115) -> ↓
// a (97) -> ←
// q (113) -> cancel
// e (101) -> ok
SerialKey serialKey(/*Up=*/119, /*Right/Next=*/100,
                    /*Down=*/115, /*Left/Prev=*/97,
                    /*Home/Cancel=*/113, /*Select/OK=*/101);


// Pins the buttons are connected to
// const byte downPin = 35;
// const byte leftPin = 36;
// const byte rightPin = 37;
// const byte upPin = 38;
// const byte cancelPin = 39;
// const byte okPin = 0;

// // Array of Key objects that will link GEM key identifiers with dedicated pins
// Key keys[] = {{GEM_KEY_UP, upPin}, {GEM_KEY_RIGHT, rightPin}, {GEM_KEY_DOWN, downPin}, {GEM_KEY_LEFT, leftPin}, {GEM_KEY_CANCEL, cancelPin}, {GEM_KEY_OK, okPin}};

// // Create KeyDetector object
// KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));



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


///////////////////////////////////////////////////////////////////////////////

// Create variables that will be editable through the menu and assign them initial values
int number = -512;
bool enablePrint = false;

// Create two menu item objects of class GEMItem, linked to number and enablePrint variables 
GEMItem menuItemInt("Number:", number);
GEMItem menuItemBool("Enable print:", enablePrint);

// Create menu button that will trigger printData() function. It will print value of our number variable
// to Serial monitor if enablePrint is true. We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void printData(); // Forward declaration
GEMItem menuItemButton("Print", printData);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("Main Menu");

// Create menu object of class GEM_adafruit_gfx. Supply its constructor with reference to tft object we created earlier
// GEM_adafruit_gfx menu(lcd_t, GEM_POINTER_ROW, GEM_ITEMS_COUNT_AUTO);
// Which is equivalent to the following call (you can adjust parameters to better fit your screen if necessary):
GEM_adafruit_gfx menu(lcd_t, /* menuPointerType= */ GEM_POINTER_ROW, /* menuItemsPerScreen= */ GEM_ITEMS_COUNT_AUTO, /* menuItemHeight= */ 8, /* menuPageScreenTopOffset= */ 10, /* menuValuesLeftOffset= */ 86);



///////////////////////////////////////////////////////////////////////////////

void update_screen() {
    lcd_t.updateWholeScreen();
}


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



    // Push-buttons pin modes
//   pinMode(downPin, INPUT);
//   pinMode(leftPin, INPUT);
//   pinMode(rightPin, INPUT);
//   pinMode(upPin, INPUT);
//   pinMode(cancelPin, INPUT);
//   pinMode(okPin, INPUT);


//   menu.setDrawMenuCallback(update_screen);


  // Menu init, setup and draw
  menu.init();

  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemBool);
  menuPageMain.addMenuItem(menuItemButton);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);

  menu.drawMenu();


}

void lcd_test() {
   // If menu is ready to accept button press...
//   if (menu.readyForKey()) {
//     // ...detect key press using KeyDetector library
//     myKeyDetector.detect();
//     // Pass pressed button to menu
//     // (pressed button ID is stored in trigger property of KeyDetector object)
//     menu.registerKeyPress(myKeyDetector.trigger);
//   }

if (menu.readyForKey()) {
    serialKey.detect();
    menu.registerKeyPress(serialKey.trigger);
  update_screen();
}

}


void printData() {
  // If enablePrint flag is set to true (checkbox on screen is checked)...
  if (enablePrint) {
    // ...print the number to Serial
    Serial.print("Number is: ");
    Serial.println(number);
  } else {
    Serial.println("Printing is disabled, sorry:(");
  }
}


///////////////////////////////////////////////////////////////////////////////
// old test code

// void lcd_test() {

//     /* draw rectangle */
//     ESP_LOGI(LOG_LCD_TAG, "LCD loop");
//     lcd_t.fillScreen(GLCD_COLOR_CLEAR);
//     lcd_t.drawRect(2, 2, 50, 20, GLCD_COLOR_SET);
//     lcd_t.updateWholeScreen();

//     delay(5000);
//     // lcd.fillScreen(GLCD_COLOR_CLEAR);
//     // delay(1000);

//     /* draw circle */
//     lcd_t.fillScreen(GLCD_COLOR_CLEAR);
//     lcd_t.drawCircle(20, 20, 10, GLCD_COLOR_SET);
//     lcd_t.updateWholeScreen();

//     delay(5000);

//     /* rotate through markers */
//     // for (int l = 0; l <= 3; l++) {
//     for (byte i = 1; i <= 6; i++) {
//         lcd_t.setMarker(i, true);
//         delay(400);
//         lcd_t.setMarker(i, false);
//         delay(100);
//         lcd_t.updateWholeScreen();
//     }
//     // }
    

//     lcd_t.fillScreen(GLCD_COLOR_CLEAR);

//     /* refresh rate test */
//     int y = 0;
//     // for (int y = 0; y < 48; y++) {
//         for (int x = 0; x < 100; x++) {
//             lcd_t.drawPixel(x, y, GLCD_COLOR_SET);
//             lcd_t.updateWholeScreen();
//         }
//     // }
    
//     delay(5000);

//     /* print text test */
//     lcd_t.fillScreen(GLCD_COLOR_CLEAR);
//     lcd_t.cp437(true);   // Use correct CP437 character codes
//     lcd_t.print("Scho"); // Print the plain ASCII first part
//     lcd_t.write(0x94);   // Print the o-with-umlauts
//     lcd_t.println("n");  // Print the last part
//     lcd_t.updateWholeScreen();

//     delay(5000);

//     /* 4x bitmaps test */

//     lcd_t.fillScreen(GLCD_COLOR_CLEAR);
//     lcd_t.setMarker(1, GLCD_COLOR_SET);
//     lcd_t.drawBitmap(0, 0, epd_bitmap_Bitmap_binary, 100, 48, GLCD_COLOR_SET);
//     lcd_t.updateWholeScreen();

//     delay(10000);

//     lcd_t.fillScreen(GLCD_COLOR_CLEAR);
//     lcd_t.setMarker(2, GLCD_COLOR_SET);
//     lcd_t.drawBitmap(0, 0, epd_bitmap_Bitmap_bayer, 100, 48, GLCD_COLOR_SET);
//     lcd_t.updateWholeScreen();

//     delay(10000);

//     lcd_t.fillScreen(GLCD_COLOR_CLEAR);
//     lcd_t.setMarker(3, GLCD_COLOR_SET);
//     lcd_t.drawBitmap(0, 0, epd_bitmap_Bitmap_floyd_steinberg, 100, 48, GLCD_COLOR_SET);
//     lcd_t.updateWholeScreen();

//     delay(10000);

//     lcd_t.fillScreen(GLCD_COLOR_CLEAR);
//     lcd_t.setMarker(4, GLCD_COLOR_SET);
//     lcd_t.drawBitmap(0, 0, epd_bitmap_Bitmap_atkinson, 100, 48, GLCD_COLOR_SET);
//     lcd_t.updateWholeScreen();

//     delay(10000);
// }
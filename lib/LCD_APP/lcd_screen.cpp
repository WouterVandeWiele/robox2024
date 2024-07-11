
#include <stdint.h>
#include "lcd_screen.h"
#include "SED1530_LCD.h"
#include "general_definitions.h"
#include "general_config.h"
#include "example_bitmaps.h"
#include "adc_key.h"

#include "robox_audio_mux.h"
extern RoboxAudioMux mux;


#if defined(LCD_RUN_THREADED)
    static LCD_Threaded lcd_t;
    static TaskHandle_t threaded_task;
#else
    static SED1530_LCD* lcd_t;
#endif


/* temporary GEM menu stuff */
#include "splash.h"
#include "robox_audio_mux.h"
// static GEMSelect menu_source_select(3, (SelectOptionByte[]){{"Bluetooth", BleSource}, {"Webradio", WebRadioSource}, {"SD card", SDSource}});

// void menu_callback_source(GEMCallbackData callbackData) {
//   Serial.print("Pressed menu item title: ");
//   Serial.println(callbackData.pMenuItem->getTitle()); // Get title of the menu item via pointer to the menu item
//   Serial.print("value: ");
//   Serial.println(callbackData.valChar); // Get user-defined variable as a part of callbackData argument
// }

// static audio_source selected_source;
// static GEMItem menuItemSelectSource("Audio Source", selected_source, menu_source_select, menu_callback_source);

void printData() {
  Serial.printf("time since start %ld\n", millis());
}
GEMItem menuItemButton("Print", printData);


void switch_to_source(GEMCallbackData source) {
    mux.switch_to((audio_source)source.valInt);
}

static int dummy_variable;

GEMItem buttonSwitchNoSource("No Source", dummy_variable, switch_to_source, (const int)NotSelectedSource);
GEMItem buttonSwitchBLE("Switch to BLE", dummy_variable, switch_to_source, (const int)BleSource);
GEMItem buttonSwitchWEB("Switch to WEB", dummy_variable, switch_to_source, (const int)WebRadioSource);
GEMItem buttonSwitchSD("Switch to SD", dummy_variable, switch_to_source, (const int)SDSource);

GEMPage menuPageMain("Main Menu");


#if defined(LCD_RUN_THREADED)
// locking primitives to communicate with the task function
static SemaphoreHandle_t xSemaphoreScreenUpdate;    // trigger to the task to update the screen, release this semafore once done (block sequential updateWholeScreen and let them wait untill the draw screen procedure has finished)
static portMUX_TYPE spinlockScreenUpdate;

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


#if defined(IO_EXPANDER)
    RoboxLcdScreen::RoboxLcdScreen(RoboxIoExpander* io): io(io) {
        lcd_t = new SED1530_LCD(io);

        // init_lcd();
        pinMode(LCD_DIS_PWR, OUTPUT);
    }
#else
    RoboxLcdScreen::RoboxLcdScreen(uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA): A0(A0), RW(RW), EN(EN), DATA(DATA), lcd_t(A0, RW, EN, DATA) {
        // init_lcd();
    }
#endif

// LCD PINS
// void lcd_screen_setup() {
// #if !defined(IO_EXPANDER) && defined(LCD_RUN_THREADED)
//     static const uint8_t lcdDataPins[] = LCDDATAPINS;

//     lcd_t = std::make_unique<LCD_Threaded>(LCDA0, LCDRW, LCDENABLE, lcdDataPins);
// #elif defined(IO_EXPANDER) && defined(LCD_RUN_THREADED)
//     lcd_t = std::make_unique<LCD_Threaded>(IO_EXPANDER_W_ADDRESS);

// #elif !defined(IO_EXPANDER) && !defined(LCD_RUN_THREADED)
//     static const uint8_t lcdDataPins[] = LCDDATAPINS;
//     lcd_t = std::make_unique<SED1530_LCD>(A0, RW, EN, DATA);

// #elif defined(IO_EXPANDER) && !defined(LCD_RUN_THREADED)
//     lcd_t = std::make_unique<SED1530_LCD>(IO_EXPANDER_W_ADDRESS);
    
// #endif
// }

#if defined(LCD_RUN_THREADED)

void LCD_Threaded::updateWholeScreen() {
    xSemaphoreTake(xSemaphoreScreenUpdate, 0);
}

void LCD_Threaded::taskUpdateWholeScreen() {
    SED1530_LCD::updateWholeScreen();
}

#endif

void update_screen() {
    lcd_t->updateWholeScreen();
}


void RoboxLcdScreen::init_lcd() {
    ESP_LOGI(LOG_LCD_TAG, "LCD setup");

    power_up();

    lcd_t->lcd_init();

    #if defined(LCD_RUN_THREADED)
    xSemaphoreScreenUpdate = xSemaphoreCreateBinary();
    spinlockScreenUpdate = portMUX_INITIALIZER_UNLOCKED;

    xTaskCreate(
        run_task,       //Function to implement the task 
        "lcd_thread", //Name of the task
        6000,       //Stack size in words 
        (void*)&lcd_t,       //Task input parameter 
        0,          //Priority of the task 
        &(threaded_task)       //Task handle.
    );
    #endif

    ESP_LOGI(LOG_LCD_TAG, "menu setup");

    menu = new GEM_adafruit_gfx(*lcd_t, /* menuPointerType= */ GEM_POINTER_ROW, /* menuItemsPerScreen= */ GEM_ITEMS_COUNT_AUTO, /* menuItemHeight= */ 8, /* menuPageScreenTopOffset= */ 10, /* menuValuesLeftOffset= */ 80);

    menu->setDrawCallback(update_screen);
    // auto _update_screen = [](RoboxLcdScreen& sc) {sc.update_screen();}; 
    // menu->setDrawCallback((void*)_update_screen);
    menu->setSplash(100, 48, robox_splash);
    menu->setSplashDelay(3000);

    menuPageMain.addMenuItem(menuItemButton);
    menuPageMain.addMenuItem(buttonSwitchNoSource);
    menuPageMain.addMenuItem(buttonSwitchBLE);
    menuPageMain.addMenuItem(buttonSwitchWEB);
    menuPageMain.addMenuItem(buttonSwitchSD);

    menu->setMenuPageCurrent(menuPageMain);
    menu->drawMenu();
}

void RoboxLcdScreen::deinit_lcd() {

}

void RoboxLcdScreen::power_up() {
    digitalWrite(LCD_DIS_PWR, HIGH);
}

void RoboxLcdScreen::power_down() {
    digitalWrite(LCD_DIS_PWR, LOW);
}

ExpanderConfig RoboxLcdScreen::io_config() {
    ExpanderConfig config = RoboxIoExpander::get_default_config();

    config.out_state[LCD_DATA_PORT] = ~LCD_DATA_PINS;
    config.out_state[LCD_CONTROL_PORT] = ~(LCD_A0 | LCD_RW | LCD_EN);

    config.out_config[LCD_DATA_PORT] = ~LCD_DATA_PINS;
    config.out_config[LCD_CONTROL_PORT] = ~(LCD_A0 | LCD_RW | LCD_EN);

    // config.drive_strength[(LCD_DATA_PORT*2)] = 0x00;
    // config.drive_strength[(LCD_DATA_PORT*2)+1] = 0x00;
    // config.drive_strength[(LCD_CONTROL_PORT*2)] = 0xFF;
    // config.drive_strength[(LCD_CONTROL_PORT*2)+1] = 0x03;

    return config;
}

void RoboxLcdScreen::io_interrupt_observer(std::vector<uint8_t>& data) {
    // RoboxLcdScreen * self = static_cast<RoboxLcdScreen*>(this_pointer);

    // implement callback code when an interrupt is generated
}


void RoboxLcdScreen::lcd_menu_loop() {
    uint8_t button;

    if (xQueueButtons != 0) {
        if(xQueueReceive( xQueueButtons, &(button), 0)) {
            
            // GEM MENU keypresses
            if ((button >= 1) && (button <= 6)) {
                // GEM KEYS (up, down, left, right, ok, cancel)
                menu->registerKeyPress(button);
            }
            else {
                // OTHER KEYS (volume up, volume down, play/pause, motor stop)
                switch (button)
                {
                case VOLUME_UP:
                    break;
                
                case VOLUME_DOWN:
                    break;
                
                case PLAY_PAUSE:
                    break;

                case MOTOR_STOP:
                    break;

                default:
                    break;
                }
            }
        }
    }
}

void RoboxLcdScreen::lcd_gfx_test() {

    /* draw rectangle */
    ESP_LOGI(LOG_LCD_TAG, "LCD loop");
    lcd_t->fillScreen(GLCD_COLOR_CLEAR);
    lcd_t->drawRect(2, 2, 50, 20, GLCD_COLOR_SET);
    lcd_t->updateWholeScreen();

    delay(5000);
    // lcd.fillScreen(GLCD_COLOR_CLEAR);
    // delay(1000);

    /* draw circle */
    lcd_t->fillScreen(GLCD_COLOR_CLEAR);
    lcd_t->drawCircle(20, 20, 10, GLCD_COLOR_SET);
    lcd_t->updateWholeScreen();

    delay(5000);

    /* rotate through markers */
    // for (int l = 0; l <= 3; l++) {
    for (byte i = 1; i <= 6; i++) {
        lcd_t->setMarker(i, true);
        delay(400);
        lcd_t->setMarker(i, false);
        delay(100);
        lcd_t->updateWholeScreen();
    }
    // }
    

    lcd_t->fillScreen(GLCD_COLOR_CLEAR);

    /* refresh rate test */
    int y = 0;
    // for (int y = 0; y < 48; y++) {
        for (int x = 0; x < 100; x++) {
            lcd_t->drawPixel(x, y, GLCD_COLOR_SET);
            lcd_t->updateWholeScreen();
        }
    // }
    
    delay(5000);

    /* print text test */
    lcd_t->fillScreen(GLCD_COLOR_CLEAR);
    lcd_t->cp437(true);   // Use correct CP437 character codes
    lcd_t->print("Scho"); // Print the plain ASCII first part
    lcd_t->write(0x94);   // Print the o-with-umlauts
    lcd_t->println("n");  // Print the last part
    lcd_t->updateWholeScreen();

    delay(5000);

    /* 4x bitmaps test */

    lcd_t->fillScreen(GLCD_COLOR_CLEAR);
    lcd_t->setMarker(1, GLCD_COLOR_SET);
    lcd_t->drawBitmap(0, 0, epd_bitmap_Bitmap_binary, 100, 48, GLCD_COLOR_SET);
    lcd_t->updateWholeScreen();

    delay(10000);

    lcd_t->fillScreen(GLCD_COLOR_CLEAR);
    lcd_t->setMarker(2, GLCD_COLOR_SET);
    lcd_t->drawBitmap(0, 0, epd_bitmap_Bitmap_bayer, 100, 48, GLCD_COLOR_SET);
    lcd_t->updateWholeScreen();

    delay(10000);

    lcd_t->fillScreen(GLCD_COLOR_CLEAR);
    lcd_t->setMarker(3, GLCD_COLOR_SET);
    lcd_t->drawBitmap(0, 0, epd_bitmap_Bitmap_floyd_steinberg, 100, 48, GLCD_COLOR_SET);
    lcd_t->updateWholeScreen();

    delay(10000);

    lcd_t->fillScreen(GLCD_COLOR_CLEAR);
    lcd_t->setMarker(4, GLCD_COLOR_SET);
    lcd_t->drawBitmap(0, 0, epd_bitmap_Bitmap_atkinson, 100, 48, GLCD_COLOR_SET);
    lcd_t->updateWholeScreen();

    delay(10000);
}
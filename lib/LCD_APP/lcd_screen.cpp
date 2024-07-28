
#include <stdint.h>
#include "lcd_screen.h"
#include "SED1530_LCD.h"
#include "general_definitions.h"
#include "general_config.h"
#include "example_bitmaps.h"
#include "adc_key.h"
#include "lcd_screen_context_loops.h"
#include "robox_audio_mux.h"

#include <WiFiManager.h>

extern RoboxAudioMux mux;
extern WiFiManager wifiManager;


// #if defined(LCD_RUN_THREADED)
//     static LCD_Threaded* lcd_t;
    static TaskHandle_t threaded_lcd_task;
// #else
    SED1530_LCD* lcd_t;
    GEM_adafruit_gfx* menu;
// #endif


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


void switch_to_no_source() {
    mux.switch_to(NotSelectedSource);
}

void switch_to_BLE() {
    mux.switch_to(BleSource);
}

void switch_to_WEB() {
    mux.switch_to(WebRadioSource);
}

void switch_to_SD() {
    mux.switch_to(SDSource);
}

GEMItem buttonSwitchNoSource("No Source", switch_to_no_source);
GEMItem buttonSwitchBLE("Sw BLE", switch_to_BLE);
GEMItem buttonSwitchWEB("Sw WEB", switch_to_WEB);
GEMItem buttonSwitchSD("Sw SD", switch_to_SD);

GEMItem menuItemSwitchAudioPlay("Play Audio", playLoop);
GEMItem menuItemSettingsAudioPlay("Play Audio", playLoop);

GEMPage menuPageSwitch("Switch Audio Source");

//////////////////////////////////////////////////////////////////////////

extern const char* wifi_ssid_2;
extern const char* wifi_password_2;

void reset_wifi_credentials() {
    uint64_t _chipmacid = 0LL;
    esp_efuse_mac_get_default((uint8_t*) (&_chipmacid));
    String hostString = String((uint32_t)_chipmacid, HEX);
    hostString.toUpperCase();
    String ssid = "ROBOX_" + hostString;


    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_OFF);
    wifiManager.resetSettings();
    wifiManager.autoConnect(ssid.c_str(), NULL);
    // WiFi.begin(wifi_ssid_2, wifi_password_2);
}

GEMItem menuPageSettingsResetWifiCredentials("Reset Wifi Cred", reset_wifi_credentials);

GEMPage menuPageSettings("Settings");


#if defined(LCD_RUN_THREADED)
// locking primitives to communicate with the task function
// static SemaphoreHandle_t xSemaphoreScreenUpdate;    // trigger to the task to update the screen, release this semafore once done (block sequential updateWholeScreen and let them wait untill the draw screen procedure has finished)
// static portMUX_TYPE spinlockScreenUpdate;

/* The index within the target task's array of task notifications to use. */
// const UBaseType_t xArrayIndex = 0;

void update_screen() {
    lcd_t->updateWholeScreen();
}


void menu_task(void * param) {
    // LCD_Threaded* p = (LCD_Threaded*)param;

    // https://www.freertos.org/RTOS_Task_Notification_As_Counting_Semaphore.html
    // BaseType_t xEvent;
    // const TickType_t xBlockTime = pdMS_TO_TICKS( 500 ); 
    // // const TickType_t xBlockTime = portMAX_DELAY ;
    // uint32_t ulNotifiedValue;

    Serial.println("menu setup");

    menu = new GEM_adafruit_gfx(
        *lcd_t, 
        /* menuPointerType= */ GEM_POINTER_ROW, 
        /* menuItemsPerScreen= */ GEM_ITEMS_COUNT_AUTO, 
        /* menuItemHeight= */ 8, 
        /* menuPageScreenTopOffset= */ 10, 
        /* menuValuesLeftOffset= */ 80
    );

    menu->setDrawCallback(update_screen);
    // menu->setSplash(100, 48, robox_splash);
    // menu->setSplashDelay(3000);

    menuPageSwitch.addMenuItem(buttonSwitchNoSource);
    menuPageSwitch.addMenuItem(buttonSwitchBLE);
    menuPageSwitch.addMenuItem(buttonSwitchWEB);
    menuPageSwitch.addMenuItem(buttonSwitchSD);
    // menuPageSwitch.addMenuItem(menuItemButton);
    menuPageSwitch.addMenuItem(menuItemSwitchAudioPlay);

    menuPageSettings.addMenuItem(menuItemButton);
    menuPageSettings.addMenuItem(menuPageSettingsResetWifiCredentials);
    menuPageSettings.addMenuItem(menuItemSettingsAudioPlay);

    menu->setMenuPageCurrent(menuPageSettings);
    // menu->init();
    // menu->drawMenu();
    playLoop();

    while (true) {

        ButtonPress button;


        if (xQueuePeek(xQueueButtons, &(button), 0)) {
            if ((menu->context.loop != nullptr) && (button.long_press == false)) {
                // let custom draw loop handle button queue
                menu->context.loop();
                // menu->readyForKey();

                if (menu->context.loop == nullptr) {
                    // draw loop has exited
                    menu->drawMenu();
                }
                // menu->registerKeyPress(GEM_KEY_NONE);
            }
            else {
                // let main GEM library handle the button queue
                if(xQueueReceive( xQueueButtons, &(button), 0)) {
                    
                    if (button.long_press == false) {
                        // GEM MENU keypresses
                        if ((button.button >= 1) && (button.button <= 6)) {
                            // GEM KEYS (up, down, left, right, ok, cancel)
                            menu->registerKeyPress(button.button);
                        }
                    }
                }
            }
        }

        delay(50);
    }
}

#endif


#if defined(IO_EXPANDER)
    RoboxLcdScreen::RoboxLcdScreen(RoboxIoExpander* io): io(io) {

        // #if defined(LCD_RUN_THREADED)
        // lcd_t = new LCD_Threaded(io);
        // #else
        lcd_t = new SED1530_LCD(io);
        // #endif

        // init_lcd();
        pinMode(LCD_DIS_PWR, OUTPUT);
    }
#else
    RoboxLcdScreen::RoboxLcdScreen(uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA): A0(A0), RW(RW), EN(EN), DATA(DATA), lcd_t(A0, RW, EN, DATA) {
        // init_lcd();
    }
#endif

void RoboxLcdScreen::init_lcd() {
    Serial.println("LCD setup");

    power_up();

    lcd_t->lcd_init();

    #if defined(LCD_RUN_THREADED)

    xTaskCreatePinnedToCore(
        menu_task,       //Function to implement the task 
        "lcd_thread", //Name of the task
        6000,       //Stack size in words 
        // (void*)&lcd_t,       //Task input parameter 
        NULL,       //Task input parameter 
        PRIORITY_LCD_TASK,          //Priority of the task 
        &(threaded_lcd_task),       //Task handle.
        1           // Core you want to run the task on (0 or 1)
    );
    #endif

}

void RoboxLcdScreen::deinit_lcd() {

}

void RoboxLcdScreen::power_up() {
    digitalWrite(LCD_DIS_PWR, LOW);
    delay(2000);
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

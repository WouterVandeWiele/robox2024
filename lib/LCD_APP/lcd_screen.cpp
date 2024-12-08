
#include <stdint.h>
#include "lcd_screen.h"
#include "SED1530_LCD.h"
#include "general_definitions.h"
#include "general_config.h"
#include "example_bitmaps.h"
#include "adc_key.h"
#include "lcd_screen_context_loops.h"
#include "robox_audio_mux.h"
#include "robox_battery.h"

#include "robox_restart.h"
#include "robox_language.h"
// extern Translator translator;

#include <WiFiManager.h>

extern RoboxAudioMux mux;
extern RoboxRestartManager restart_manager;
extern RoboxBattery* battery;

extern bool is_ble_connected;
extern bool is_ble_on;

#if defined(ROBOX_WIFI_MANAGER)
extern WiFiManager wifiManager;
#endif

// #if defined(LCD_RUN_THREADED)
//     static LCD_Threaded* lcd_t;
    static TaskHandle_t threaded_lcd_task;
    static TaskHandle_t threaded_marker_task;
// #else
    SED1530_LCD* lcd_t;
    GEM_adafruit_gfx* menu;
// #endif

float _speed_motor_left = 0.0f;
float _speed_motor_right = 0.0f;
float _speed_motor_test = 0.0f;
float _led_dimmer = 1.0f;

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
GEMItem menuItemButton(LANG_PRINT, printData);


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

void set_motor_left_speed(GEMCallbackData data) {
    // mux.switch_to(SDSource);
    Serial.printf("motor left speed: %f\n", _speed_motor_left);
    restart_manager.set_motor_speed_left(_speed_motor_left);
}

void set_motor_right_speed(GEMCallbackData data) {
    // mux.switch_to(SDSource);
    Serial.printf("motor right speed: %f\n", _speed_motor_right);
    restart_manager.set_motor_speed_right(_speed_motor_right);
}

void set_led_dimmer(GEMCallbackData data) {
    // mux.switch_to(SDSource);
    Serial.printf("led dimmer: %f\n", _led_dimmer);
    restart_manager.set_led_dimmer(_led_dimmer);
}


static GEMItem buttonSwitchNoSource(LANG_SOURCE_NO, switch_to_no_source);
static GEMItem buttonSwitchBLE(LANG_SOURCE_BLE, switch_to_BLE);
static GEMItem buttonSwitchWEB(LANG_SOURCE_WEB, switch_to_WEB);
static GEMItem buttonSwitchSD(LANG_SOURCE_SD, switch_to_SD);
static GEMItem menuItemSwitchAudioPlay(LANG_BACK, playLoop);
static GEMItem menuItemSettingsAudioPlay(LANG_BACK, playLoop);

static SelectOptionFloat selectSpeedOptions[] = {{"60", 0.60f}, {"58", 0.58f}, {"56", 0.56f}, {"54", 0.54f}, {"52", 0.52f}, {"50", 0.50f}, {"48", 0.48f}, {"46", 0.46f}, {"44", 0.44f}, {"42", 0.42f}, {"40", 0.40f}, {"38", 0.38f}, {"36", 0.36f}, {"34", 0.34f}, {"32", 0.32f}, {"30", 0.30f}};
static GEMSelect selectSpeedScheme(sizeof(selectSpeedOptions)/sizeof(SelectOptionFloat), selectSpeedOptions);
// static GEMItem motorLeftSpeed(LANG_MOTOR_SPEED_LEFT, _speed_motor_left, selectSpeedScheme, set_motor_left_speed);
// static GEMItem motorLeftSpeed(LANG_MOTOR_SPEED_RIGHT, _speed_motor_right, selectSpeedScheme, set_motor_test_speed);
static GEMItem motorLeftSpeed(LANG_MOTOR_SPEED_LEFT, _speed_motor_left, selectSpeedScheme, set_motor_left_speed);
static GEMItem motorRightSpeed(LANG_MOTOR_SPEED_RIGHT, _speed_motor_right, selectSpeedScheme, set_motor_right_speed);

static SelectOptionFloat selectLightOptions[] = {{"max", 1.0f}, {"95", 0.95f}, {"90", 0.90f}, {"85", 0.85f}, {"80", 0.80f}, {"75", 0.75f}, {"70", 0.70f}, {"65", 0.65f}, {"60", 0.60f}, {"55", 0.55f}, {"50", 0.50f}, {"45", 0.45f}, {"40", 0.40f}, {"35", 0.35f}, {"30", 0.30f}, {"25", 0.25f}, {"20", 0.20f}, {"15", 0.15f}, {"10", 0.10f}, {"5", 0.05f}};
static GEMSelect selectLightScheme(sizeof(selectLightOptions)/sizeof(SelectOptionFloat), selectLightOptions);
static GEMItem ledDimmer(LANG_LED_DIMMER, _led_dimmer, selectLightScheme, set_led_dimmer);

GEMPage menuPageSwitch(LANG_MENU_SWITCH);


void dummy() {};

static bool first_update = false;

char* item_ssid;

static uint32_t dummy_counter = 0;
static GEMItem menuROstart1("", dummy);
static GEMItem menuROstart2(LANG_RO_RO, dummy);
static GEMItem menuROssid_label(LANG_RO_WIFI_SSID, dummy);
static GEMItem menuROssid_value(String("ROBOX").c_str(), dummy);
static GEMItem menuRObreak1("     ~~~~     ", dummy);
static GEMItem menuROble_label(LANG_RO_BLE_NAME, dummy);
static GEMItem menuROble_value(String("ROBOX_bb").c_str(), dummy);
static GEMItem menuRObreak2("     ~~~~     ", dummy);
static GEMItem menuROip_label(LANG_RO_IP_ADDRESS, dummy);
static GEMItem menuROip_value(String("ROBOX_bb").c_str(), dummy);
static GEMItem menuRObreak3("     ~~~~     ", dummy);
static GEMItem menuRObattery_voltage_label(LANG_RO_BATTERY_VOLTAGE, dummy);
static GEMItem menuRObattery_voltage_value(String("ROBOX_bb").c_str(), dummy);
static GEMItem menuRObreak4("     ~~~~     ", dummy);
static GEMItem menuROversion_label(LANG_RO_ROBOX_VERSION, dummy);
static GEMItem menuROversion_value(ROBOX_VERSION, dummy);

//////////////////////////////////////////////////////////////////////////

// void reset_wifi_credentials() {
//     playLoop();
//     restart_manager.resetWifiCred();
//     restart_manager.setWifiSetupText();
//     restart_manager.setupWifiOnDemand();
// }

// static GEMItem menuPageSettingsResetWifiCredentials(LANG_RESET_WIFI_CRED, reset_wifi_credentials);

GEMPage menuPageSettings(LANG_MENU_SETTINGS);

GEMItem menuAudioControlBack(LANG_BACK, playLoop);
std::vector<GEMItem*> audioControlItems;
GEMPage menuAudioControl(LANG_MENU_AUDIO_CONTROLS);


#if defined(LCD_RUN_THREADED)
// locking primitives to communicate with the task function
// static SemaphoreHandle_t xSemaphoreScreenUpdate;
// trigger to the task to update the screen, release this semafore once done (block sequential updateWholeScreen
// and let them wait untill the draw screen procedure has finished)
// static portMUX_TYPE spinlockScreenUpdate;

/* The index within the target task's array of task notifications to use. */
// const UBaseType_t xArrayIndex = 0;

void update_screen() {
    lcd_t->updateWholeScreen();
}


[[noreturn]] void menu_task(void * param) {
    // LCD_Threaded* p = (LCD_Threaded*)param;

    // https://www.freertos.org/RTOS_Task_Notification_As_Counting_Semaphore.html
    // BaseType_t xEvent;
    // const TickType_t xBlockTime = pdMS_TO_TICKS( 500 ); 
    // // const TickType_t xBlockTime = portMAX_DELAY ;
    // uint32_t ulNotifiedValue;

    ButtonPress button;

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
    menu->hideVersion();

    if (restart_manager.is_cold_boot() == true) {
        menu->setSplashDelay(2000);
        menu->setSplash(100, 48, robox_splash);
    }
    else {
        menu->setSplashDelay(2);
        menu->setSplash(100, 48, empty_splash);
    }

    menuPageSwitch.addMenuItem(buttonSwitchNoSource);
    menuPageSwitch.addMenuItem(buttonSwitchBLE);
    menuPageSwitch.addMenuItem(buttonSwitchWEB);
    menuPageSwitch.addMenuItem(buttonSwitchSD);
    // menuPageSwitch.addMenuItem(menuItemButton);
    menuPageSwitch.addMenuItem(menuItemSwitchAudioPlay);

    // menuPageSettings.addMenuItem(menuItemButton);
    // #if defined(ROBOX_WIFI_MANAGER)
    // menuPageSettings.addMenuItem(menuPageSettingsResetWifiCredentials);
    // #endif
    menuPageSettings.addMenuItem(menuItemSettingsAudioPlay);

    menuPageSettings.addMenuItem(motorLeftSpeed);
    menuPageSettings.addMenuItem(motorRightSpeed);
    menuPageSettings.addMenuItem(ledDimmer);

    // Read only menu
    menuPageSettings.addMenuItem(menuROstart1);
    menuPageSettings.addMenuItem(menuROstart2);

    menuPageSettings.addMenuItem(menuROssid_label);
    menuPageSettings.addMenuItem(menuROssid_value);
    menuPageSettings.addMenuItem(menuRObreak1);

    menuPageSettings.addMenuItem(menuROble_label);
    menuPageSettings.addMenuItem(menuROble_value);
    menuPageSettings.addMenuItem(menuRObreak2);

    menuPageSettings.addMenuItem(menuROip_label);
    menuPageSettings.addMenuItem(menuROip_value);
    menuPageSettings.addMenuItem(menuRObreak3);

    menuPageSettings.addMenuItem(menuRObattery_voltage_label);
    menuPageSettings.addMenuItem(menuRObattery_voltage_value);

    menuPageSettings.addMenuItem(menuRObreak4);

    menuPageSettings.addMenuItem(menuROversion_label);
    menuPageSettings.addMenuItem(menuROversion_value);

    // for (auto i : mux.song_list()) {
    //     audioControlItems.push_back(new GEMItem(i, dummy, true));
    //     menuAudioControl.addMenuItem(*audioControlItems.back());
    // }

    menuAudioControl.addMenuItem(menuAudioControlBack);

    menu->setMenuPageCurrent(menuPageSettings);
    menu->init();
    
    playLoop();

    for (;;) {
        if (xQueueButtons == NULL) {
            continue;
        }
      
        if (!menu->readyForKey()) {
            continue;
        }

        for (;;) {
            if (!xQueueReceive(xQueueButtons, &button, 100 * portTICK_PERIOD_MS)) {
                continue;
            }
            if (button.long_press) {
                continue;
            }
            if (button.button == GEM_PLAY_MENU) {
                playLoop();
                break;
            }
            if (button.button < GEM_KEY_UP || button.button > GEM_KEY_OK) {
                continue;
            }
            break;
        }
        // Serial.println("Got button press");
        // menuTestChangeLabel.setTitle("Counter");

        // Serial.printf("- %s\n", menuROssid_value.getTitle().c_str());
        // Serial.printf("- %s\n", menuROble_value.getTitle().c_str());
        // Serial.printf("- %s\n", menuROip_value.getTitle().c_str());
        // Serial.printf("- %s\n", menuRObattery_voltage_value.getTitle().c_str());

        menuROssid_value.setTitle(restart_manager.getDefaultName());
        menuROble_value.setTitle(restart_manager.getDefaultName());
        menuROip_value.setTitle(WiFi.localIP().toString());
        menuRObattery_voltage_value.setTitle( (String((float)battery->battery_voltage()/1000)+"V"));

        menu->registerKeyPress(button.button);
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


[[noreturn]] void marker_task(void * param) {
    BatteryData battery;

    bool maker_toggle = false;
    BatteryState battery_marker = battery_high;
    bool is_charging = false;
    bool is_charge_stby = false;

    pinMode(USD_CARD_DETECT, INPUT);

    while (true)
    {
        delay(1000);

        if (xQueueBattery == NULL) {
            continue;
        }

        // battery indicators
        if (xQueueReceive(xQueueBattery, &battery, 0)) {
            battery_marker = battery.state;
            is_charging = battery.chargerCharging;
            is_charge_stby = battery.chargerChgStBy;

        }

        // battery indicator
        if (battery_marker == battery_low) {
            lcd_t->setMarker(GLCD_MARKER_BATTERY, true);
        }
        else if (battery_marker == battery_high) {
            lcd_t->setMarker(GLCD_MARKER_BATTERY, false);
        }
        if (battery_marker == battery_verylow) {
            lcd_t->setMarker(GLCD_MARKER_BATTERY, maker_toggle);
        }

        // charge indicator
        // Serial.printf("is charging %d, is standby: %d\n", is_charging ? 1 : 0, is_charge_stby ? 1 : 0);
        if (!is_charge_stby) {
            lcd_t->setMarker(GLCD_MARKER_STAR, true);
        }
        else if (!is_charging) {
            lcd_t->setMarker(GLCD_MARKER_STAR, maker_toggle);
        } 
        else {
            lcd_t->setMarker(GLCD_MARKER_STAR, false);
        }

        // SD card detect
        int sd_detect = digitalRead(USD_CARD_DETECT);
        // Serial.printf("SD detect %d\n", sd_detect);
        if (!sd_detect) {
            lcd_t->setMarker(GLCD_MARKER_ARROW_UP, true);
        }
        else {
            if (mux.get_current_source() == SDSource) {
                lcd_t->setMarker(GLCD_MARKER_ARROW_UP, maker_toggle);
            }
            else {
                lcd_t->setMarker(GLCD_MARKER_ARROW_UP, false);
            }
        }

        // wifi status indicator
        if (restart_manager.is_wifi_started()) {
            if (restart_manager.is_wifi_initialized()){
                lcd_t->setMarker(GLCD_MARKER_ARROWS_CROSS, true);
            }
            else {
                lcd_t->setMarker(GLCD_MARKER_ARROWS_CROSS, maker_toggle);
            }
        }
        else {
            lcd_t->setMarker(GLCD_MARKER_ARROWS_CROSS, false);
        }

        // ble status indicator

        if (is_ble_on) {
            lcd_t->setMarker(GLCD_MARKER_BARCODE, true);
        }
        else {
            lcd_t->setMarker(GLCD_MARKER_BARCODE, false);
        }
        if (is_ble_connected) {
            lcd_t->setMarker(GLCD_MARKER_BARCODE_CROSS, true);
        }
        else {
            lcd_t->setMarker(GLCD_MARKER_BARCODE_CROSS, false);
        }

        maker_toggle = !maker_toggle;
    }
    
}

void RoboxLcdScreen::init_lcd() {
    Serial.println("LCD setup");

    if (restart_manager.is_cold_boot()) {
        power_up();

        lcd_t->lcd_init();
    }

    _speed_motor_left = restart_manager.get_motor_speed_left();
    _speed_motor_right = restart_manager.get_motor_speed_right();
    _led_dimmer = restart_manager.get_led_dimmer();

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

    xTaskCreatePinnedToCore(
        marker_task,       //Function to implement the task 
        "marker_thread", //Name of the task
        3000,       //Stack size in words 
        // (void*)&lcd_t,       //Task input parameter 
        NULL,       //Task input parameter 
        PRIORITY_LCD_TASK,          //Priority of the task 
        &(threaded_marker_task),       //Task handle.
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

#include <Arduino.h>
#include "AudioTools.h"

#include "robox_fft_beat.h"

#include "hal/adc_types.h"

/*
 * Compile parts of the project:
 * - ROBOX_FULL: the complete audio project, with audio mux
 * - ROBOX_EXAMPLE_*: the examples, from the arduino audio library, with minimal adaptations
 * - ROBOX_COMPONENT_*: the examples, from the arduino audio library, with adaptations for the audio mux
 *
 * Select only one of the above
 */

#define ROBOX_FULL

// #define ROBOX_EXAMPLE_BLE
// #define ROBOX_EXAMPLE_BLE_BEAT
// #define ROBOX_EXAMPLE_SD
// #define ROBOX_EXAMPLE_SD_PLAYER_BEAT
// #define ROBOX_EXAMPLE_WEB
// #define ROBOX_EXAMPLE_WEB_PLAYER_BEAT
// #define ROBOX_EXAMPLE_MULTI_WEB_FFT

// #define ROBOX_COMPONENT_BLE
// #define ROBOX_COMPONENT_WEB
// #define ROBOX_COMPONENT_SD


/*
 * External parts:
 * ROBOX_LCD: include the LCD component (headers, init and loop)
 * ROBOX_DEBUG_CLI: include the debug CLI component (headers, init and loop)
 * ROBOX_DEBUG_I2C: test the I2C IO expander component
 * ROBOX_DEBUG_I2C_SCANNER: test the I2C, do a scan to detect which HW addresses are active.
 * ROBOX_PREFERENCES: initialize ESP32 NVS as Arduino Preferences
 *
 * Multiple can be selected
 */

#define ROBOX_LCD
#define ROBOX_BATTERY
#define ROBOX_MOTOR
#define ROBOX_TEST_ADC_KEY
// #define ROBOX_DEBUG_CLI
// #define ROBOX_DEBUG_I2C
// #define ROBOX_DEBUG_I2C_SCANNER
#define ROBOX_PREFERENCES
// #define ROBOX_WIFI_MANAGER
// #define ROBOX_WIFI
// #define ROBOX_IMPROV
// #define ROBOX_SERVER
#define ROBOX_RESTART
#define ROBOX_TEST_ADC

// #include "robox_language.h"
// Translator translator(lang_en);

/*
 * compile options logic
 */

#if defined(ROBOX_FULL)
    #include "robox_audio_mux.h"
    extern bool is_ble_connected;
    extern bool is_ble_on;

#elif defined(ROBOX_COMPONENT_BLE)
    #include "robox_ble.h"

#elif defined(ROBOX_COMPONENT_WEB)
    #include "robox_web.h"

#elif defined(ROBOX_COMPONENT_SD)
    #include "robox_sd.h"

#elif defined(ROBOX_EXAMPLE_BLE)
    #include "ble_example.h"

#elif defined(ROBOX_EXAMPLE_BLE_BEAT)
    #include "beat_a2dp_multi.h"

#elif defined(ROBOX_EXAMPLE_SD)
    #include "sdfat_example.h"

#elif defined(ROBOX_EXAMPLE_SD_PLAYER_BEAT)
    #include "beat_sd_player_multi.h"

#elif defined(ROBOX_EXAMPLE_WEB)
    // #include "web_radio.h"
    // #include "web_example_meta2.h"
    #include "player_metadata.h"

#elif defined(ROBOX_EXAMPLE_WEB_PLAYER_BEAT)
    #include "beat_web_player_multi.h"

#elif defined(ROBOX_EXAMPLE_MULTI_WEB_FFT)
    #include "simple_fft_example2.h"

#endif

#if defined(ROBOX_LCD)
    // #include "lcd_config.h"
    #include "lcd_screen.h"
#endif

#if defined(ROBOX_MOTOR)
    #include "robox_motor.h"
#endif

#if defined(ROBOX_TEST_ADC_KEY)
    #include "adc_key.h"
#endif

#if defined(ROBOX_BATTERY)
    #include "robox_battery.h"
#endif

#if defined(ROBOX_DEBUG_CLI)
    #include "debug_cli.h"
#endif

#if defined(ROBOX_DEBUG_I2C)
    #include "robox_io.h"
#endif

#if defined(ROBOX_DEBUG_I2C_SCANNER)
    #include "robox_i2c_scanner.h"
#endif

#if defined(ROBOX_PREFERENCES)
    #include <Preferences.h>
    #define RW_MODE false
    #define RO_MODE true
#endif

#if defined(ROBOX_WIFI_MANAGER)
    #include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
    #include <ESPmDNS.h>
    #include "esp_mac.h"
    // #include <WiFi.h>

    // //needed for library
    // #include <ESPAsyncWebServer.h>
    // #include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager
#endif

#if defined(ROBOX_WIFI)
    #include "robox_wifi.h"
#endif

#if defined(ROBOX_IMPROV)
    #include <ImprovWiFiLibrary.h>
    ImprovWiFi improvSerial(&Serial);
#endif

#if defined(ROBOX_SERVER)
    #include "robox_server.h"
#endif

#if defined(ROBOX_RESTART)
    #include "robox_restart.h"
    RoboxRestartManager restart_manager;
#endif

#include "robox_led_motor_controller.h"
LedMotorController led_motor_controller;

#if defined(ROBOX_TEST_ADC)
int adc2_pin13;
uint64_t adc_timekeeper = 5000;
extern uint32_t beat_loop_timestamp;
static uint32_t beat_loop_timestamp_previous = 0;
#endif

// #include "ble_example.h"
// #include "ble_copier.h"
// #include "streams_copy_example.h"
// #include "listing.h"
#include "general_definitions.h"
// #include "wifi_credentials.h"

// setup logging
// #define LOG_LOCAL_LEVEL ESP_LOG_ERROR
#include "esp_log.h"
// #include "esp32-hal-log.h"

// #define MP3_MAX_OUTPUT_SIZE 2048*5//1024 * 5
// #define MP3_MAX_FRAME_SIZE 65536 //1600

// #define DEFAULT_BUFFER_SIZE 4096 //1024
// #define I2S_BUFFER_SIZE 1024 //512

// #define PREFER_FIXEDPOINT 

// typedef int16_t sound_t;                                   // sound will be represented as int16_t (with 2 bytes)



#if defined(ROBOX_FULL)
    RoboxAudioMux mux;
    static TaskHandle_t AudioCopyTask;

#elif defined(ROBOX_COMPONENT_BLE)
    RoboxBluetooth ble;

#elif defined(ROBOX_COMPONENT_WEB)
    RoboxWebRadio web;

#elif defined(ROBOX_COMPONENT_SD)
    RoboxSD sd;

#endif

#if defined(ROBOX_DEBUG_I2C) || defined(ROBOX_LCD) || defined(ROBOX_MOTOR) || defined(ROBOX_BATTERY)
static RoboxIoExpander* io;
#endif

#if defined(ROBOX_LCD)
static RoboxLcdScreen* screen;
#endif

#if defined(ROBOX_MOTOR)
RoboxMotor* motor;
unsigned long timekeeper = 0;
uint8_t motor_test_program = 0;
#define MOTOR_TEST_PROGRAMS 4
#endif

#if defined(ROBOX_BATTERY)
RoboxBattery* battery;
#endif


#if defined(ROBOX_PREFERENCES)
Preferences roboxPrefs;
const char* wifi_ssid_2;
const char* wifi_password_2;
bool motorsOn;
#endif

#if defined(ROBOX_WIFI_MANAGER)
    WiFiManager wifiManager;

    unsigned int  timeout   = 120; // seconds to run for
    unsigned int  startTime = millis();
    bool portalRunning      = false;
    bool startAP            = false; // start AP and webserver if true, else start only webserver

    // static AsyncWebServer server_manager(80);
    // static DNSServer dns;
    // static AsyncWiFiManager wifiManager(&server_manager,&dns);

    void doWiFiManager(){
  // is auto timeout portal running
  if(portalRunning){
    wifiManager.process(); // do processing

    // check for timeout
    if((millis()-startTime) > (timeout*1000)){
      Serial.println("portaltimeout");
      portalRunning = false;
      if(startAP){
        wifiManager.stopConfigPortal();
      }
      else{
        wifiManager.stopWebPortal();
      } 
   }
  }

  // is configuration portal requested?
  if(!portalRunning) {
    if(startAP){
      Serial.println("Button Pressed, Starting Config Portal");
      wifiManager.setConfigPortalBlocking(false);
      wifiManager.startConfigPortal();
    }  
    else{
      Serial.println("Button Pressed, Starting Web Portal");
      wifiManager.startWebPortal();
    }  
    portalRunning = true;
    startTime = millis();
  }
}
#endif


[[noreturn]] void audio_task(void* parameter) {
    mux.setup();

    while (true) {
        mux.copy();
        vTaskDelay(1);
    }
}

void setup() {
    Serial.begin(115200);

    AudioLogger::instance().begin(Serial, AudioLogger::Info);

    // font generated with https://patorjk.com/software/taag/#p=display&f=Doh&t=ROBOX

    Serial.println("\n");
    Serial.println("RRRRRRRRRRRRRRRRR        OOOOOOOOO     BBBBBBBBBBBBBBBBB        OOOOOOOOO     XXXXXXX       XXXXXXX");
    Serial.println("R::::::::::::::::R     OO:::::::::OO   B::::::::::::::::B     OO:::::::::OO   X:::::X       X:::::X");
    Serial.println("R::::::RRRRRR:::::R  OO:::::::::::::OO B::::::BBBBBB:::::B  OO:::::::::::::OO X:::::X       X:::::X");
    Serial.println("RR:::::R     R:::::RO:::::::OOO:::::::OBB:::::B     B:::::BO:::::::OOO:::::::OX::::::X     X::::::X");
    Serial.println("  R::::R     R:::::RO::::::O   O::::::O  B::::B     B:::::BO::::::O   O::::::OXXX:::::X   X:::::XXX");
    Serial.println("  R::::R     R:::::RO:::::O     O:::::O  B::::B     B:::::BO:::::O     O:::::O   X:::::X X:::::X   ");
    Serial.println("  R::::RRRRRR:::::R O:::::O     O:::::O  B::::BBBBBB:::::B O:::::O     O:::::O    X:::::X:::::X    ");
    Serial.println("  R:::::::::::::RR  O:::::O     O:::::O  B:::::::::::::BB  O:::::O     O:::::O     X:::::::::X     ");
    Serial.println("  R::::RRRRRR:::::R O:::::O     O:::::O  B::::BBBBBB:::::B O:::::O     O:::::O     X:::::::::X     ");
    Serial.println("  R::::R     R:::::RO:::::O     O:::::O  B::::B     B:::::BO:::::O     O:::::O    X:::::X:::::X    ");
    Serial.println("  R::::R     R:::::RO:::::O     O:::::O  B::::B     B:::::BO:::::O     O:::::O   X:::::X X:::::X   ");
    Serial.println("  R::::R     R:::::RO::::::O   O::::::O  B::::B     B:::::BO::::::O   O::::::OXXX:::::X   X:::::XXX");
    Serial.println("RR:::::R     R:::::RO:::::::OOO:::::::OBB:::::BBBBBB::::::BO:::::::OOO:::::::OX::::::X     X::::::X");
    Serial.println("R::::::R     R:::::R OO:::::::::::::OO B:::::::::::::::::B  OO:::::::::::::OO X:::::X       X:::::X");
    Serial.println("R::::::R     R:::::R   OO:::::::::OO   B::::::::::::::::B     OO:::::::::OO   X:::::X       X:::::X");
    Serial.println("RRRRRRRR     RRRRRRR     OOOOOOOOO     BBBBBBBBBBBBBBBBB        OOOOOOOOO     XXXXXXX       XXXXXXX");
    Serial.println("\n");
    Serial.println("Version: 2024.08.11\n");
    Serial.println("More info on https://woutervandewiele.github.io/robox2024/\n");
    Serial.println("\n");

    // setup logging
    esp_log_level_set("*", ESP_LOG_ERROR);
    // esp_log_level_set("*", ESP_LOG_WARN);
    // esp_log_level_set(LOG_MAIN_TAG, ESP_LOG_DEBUG);
    // esp_log_level_set(LOG_BLE_TAG, ESP_LOG_DEBUG);
    // esp_log_level_set(LOG_MUX_TAG, ESP_LOG_DEBUG);
    // esp_log_level_set(LOG_I2S_TAG, ESP_LOG_DEBUG);
    esp_log_level_set(LOG_I2C_TAG, ESP_LOG_DEBUG);
    // esp_log_level_set(LOG_SD_TAG, ESP_LOG_DEBUG);
    // esp_log_level_set(LOG_WEB_TAG, ESP_LOG_DEBUG);

    // esp_log_level_set(BT_AV_TAG, ESP_LOG_NONE);
    // esp_log_level_set(BT_APP_TAG, ESP_LOG_NONE);

    // init after restart
    led_motor_controller.set_current(restart_manager.get_led_motor());

    // fastled setup
    led_init();

    #if defined(ROBOX_PREFERENCES)
    roboxPrefs.begin("roboxPrefs", RO_MODE); 
    
    bool nvsInit = roboxPrefs.isKey("nvsInit");       // Test for the existence
                                                      // of the "already initialized" key.

    if (nvsInit == false) {
      // If nvsInit is 'false', the key "nvsInit" does not yet exist therefore this
      //  must be our first-time run. We need to set up our Preferences namespace keys. So...
      roboxPrefs.end();                             // close the namespace in RO mode and...
      roboxPrefs.begin("roboxPrefs", RW_MODE);      //  reopen it in RW mode.

      // The .begin() method created the "roboxPrefs" namespace and since this is our
      // first-time run we will create
      // our keys and store the initial "factory default" values.

      roboxPrefs.putString("wifi_ssid_2", "fri3d-badge");
      roboxPrefs.putString("wifi_password_2", "badge2024");
      roboxPrefs.putBool("motorsOn", true);

      roboxPrefs.putBool("nvsInit", true);      // Create the "already initialized"
                                                //  key and store a value.

      // The "factory defaults" are created and stored so...
      roboxPrefs.end();                             // Close the namespace in RW mode and...
      roboxPrefs.begin("roboxPrefs", RO_MODE);      //  reopen it in RO mode so the setup code
                                                    //  outside this first-time run 'if' block
                                                    //  can retrieve the run-time values
                                                    //  from the "roboxPrefs" namespace.
    }

    // Retrieve the operational parameters from the namespace
    //  and save them into their run-time variables.
    wifi_ssid_2 = roboxPrefs.getString("wifi_ssid_2").c_str();            //  The LHS variables were defined
    wifi_password_2 = roboxPrefs.getString("wifi_password_2").c_str();    //   earlier in the sketch.
    motorsOn = roboxPrefs.getBool("motorsOn");                //

    // All done. Last run state (or the factory default) is now restored.
    roboxPrefs.end();                                      // Close our preferences namespace.
    #endif

    #if defined(ROBOX_FULL)
        ESP_LOGI(LOG_MAIN_TAG, "Setup mux");
        // mux.setup();
        // mux.switch_to(BleSource);
        // mux.switch_to(WebRadioSource);
        // mux.switch_to(SDSource);

        // xTaskCreatePinnedToCore(
        //     audio_task,       //Function to implement the task 
        //     "audio_task", //Name of the task
        //     6000,       //Stack size in words 
        //     NULL,       //Task input parameter 
        //     7,          //Priority of the task 
        //     &AudioCopyTask,       //Task handle.
        //     1           // Core you want to run the task on (0 or 1)
        // );

    #elif defined(ROBOX_COMPONENT_BLE)
        ESP_LOGI(LOG_MAIN_TAG, "ble start");
        ble.mux_start();

    #elif defined(ROBOX_COMPONENT_WEB)
        ESP_LOGI(LOG_MAIN_TAG, "web start");
        web.mux_start();

    #elif defined(ROBOX_COMPONENT_SD)
        ESP_LOGI(LOG_MAIN_TAG, "sd start");
        sd.mux_start();

    #elif defined(ROBOX_EXAMPLE_BLE) || defined(ROBOX_EXAMPLE_BLE_BEAT) || defined(ROBOX_EXAMPLE_SD) || defined(ROBOX_EXAMPLE_WEB) || defined(ROBOX_EXAMPLE_MULTI_WEB_FFT) || defined(ROBOX_EXAMPLE_WEB_PLAYER_BEAT) || defined(ROBOX_EXAMPLE_SD_PLAYER_BEAT)
        ESP_LOGI(LOG_MAIN_TAG, "examples start");
        player_setup();

    #endif


    #if defined(ROBOX_DEBUG_I2C) || defined(ROBOX_LCD) || defined(ROBOX_MOTOR) || defined(ROBOX_BATTERY)
    io = new RoboxIoExpander(IO_EXPANDER_W_ADDRESS);
    #endif


    #if defined(ROBOX_LCD)
        ESP_LOGI(LOG_MAIN_TAG, "lcd setup");

        screen = new RoboxLcdScreen(io);
        io->register_observer(screen, RoboxLcdScreen::io_config());
    #endif

    #if defined(ROBOX_MOTOR)
        ESP_LOGI(LOG_MAIN_TAG, "motor setup");
        motor = new RoboxMotor(io);
        io->register_observer(motor, RoboxMotor::io_config());
    #endif

    #if defined(ROBOX_BATTERY)
        ESP_LOGI(LOG_MAIN_TAG, "battery setup");
        battery = new RoboxBattery(io);
        io->register_observer(battery, RoboxBattery::io_config());
    #endif

    #if defined(ROBOX_DEBUG_I2C) || defined(ROBOX_LCD) || defined(ROBOX_MOTOR)
        ESP_LOGI(LOG_MAIN_TAG, "configure io-expander");
        io->io_configure();
    #endif

    #if defined(ROBOX_LCD)
        ESP_LOGI(LOG_MAIN_TAG, "lcd init");
        screen->init_lcd();
    #endif

    #if defined(ROBOX_BATTERY)
        battery->initBattery();
    #endif

    #if defined(ROBOX_DEBUG_CLI)
        ESP_LOGI(LOG_MAIN_TAG, "select ble source");
        debug_cli_setup();
    #endif

    #if defined(ROBOX_DEBUG_I2C)
        io->configure_outputs(0, 0x00);
        io->configure_outputs(1, 0x00);
    #endif

    #if defined(ROBOX_DEBUG_I2C_SCANNER)
        scanner_setup();
    #endif

    #if defined(ROBOX_TEST_ADC_KEY)
        adc_key_setup();
    #endif

    #if defined(ROBOX_MOTOR)
    motor->init();
    #endif


    xTaskCreatePinnedToCore(
        audio_task,       //Function to implement the task 
        "audio_task", //Name of the task
        6000,       //Stack size in words 
        NULL,       //Task input parameter 
        PRIORITY_AUDIO_TASK,          //Priority of the task 
        &AudioCopyTask,       //Task handle.
        0           // Core you want to run the task on (0 or 1)
    );

    #if defined(ROBOX_WIFI)
        wifi_setup();
    #endif

    #if defined(ROBOX_IMPROV)
        improvSerial.setDeviceInfo(
            ImprovTypes::ChipFamily::CF_ESP32,  // chipFamily
            "Robox-2024",                       // firmware name
            "0.0.1",                            // firmware version
            "Robox 2024"                        // device name
        );
    #endif
    

    // server_setup();
    // server_start();

    restart_manager.setupWifi();

}

void loop() {
    #if defined(ROBOX_TEST_ADC)

    if (millis() > adc_timekeeper) {
        // esp_err_t r = adc2_get_raw(ADC2_CHANNEL_4, ADC_WIDTH_BIT_12, &adc2_pin13);
        // Serial.printf("adc2 pin13[%s]: %d\n", (r == ESP_OK) ? "ADC OK" : "FAIL", adc2_pin13);
        // adc1_config_width(ADC_WIDTH_BIT_12);
        // adc2_pin13 = adc1_get_raw(ADC1_CHANNEL_3);//, ADC_WIDTH_BIT_12, &adc2_pin13);
        // Serial.printf("adc1 pin39[]: %d\n", adc2_pin13);

        // Serial.printf("adc1 pin 39: %ld\n", analogReadMilliVolts(39));
        // Serial.printf("mux last %ld\n", beat_loop_timestamp);

        if (beat_loop_timestamp_previous == beat_loop_timestamp) {
            // Serial.println("no audio playing");

            motor->shutdown_idempotent();
        } 
        else {
            if (led_motor_controller.is_motor_enabled()) {
                motor->enable_idempotent();
            }
            // Serial.println("audio playing");
        }

        beat_loop_timestamp_previous = beat_loop_timestamp;
        adc_timekeeper = millis() + 500;
    }
    #endif


    #if defined(ROBOX_FULL)
        // mux.copy();
    
    #elif defined(ROBOX_EXAMPLE_BLE) || defined(ROBOX_EXAMPLE_BLE_BEAT) || defined(ROBOX_EXAMPLE_SD) || defined(ROBOX_EXAMPLE_WEB) || defined(ROBOX_EXAMPLE_MULTI_WEB_FFT) || defined(ROBOX_EXAMPLE_WEB_PLAYER_BEAT) || defined(ROBOX_EXAMPLE_SD_PLAYER_BEAT)
        player_loop();

    #elif defined(ROBOX_COMPONENT_BLE)
        ble.mux_copy();

    #elif defined(ROBOX_COMPONENT_WEB)
        web.mux_copy();

    #elif defined(ROBOX_COMPONENT_SD)
        sd.mux_copy();

    #endif


    // #if defined(ROBOX_LCD)
    //     // ESP_LOGI(LOG_MAIN_TAG, "LCD loop");

    //     // screen->lcd_gfx_test();
    //     screen->lcd_menu_loop();
    // #endif

    #if defined(ROBOX_DEBUG_CLI)
        debug_cli_loop();
    #endif

    // #if defined(ROBOX_MOTOR) || defined(ROBOX_BATTERY)
    //     // check interrupt
    //     io->loop();
    // #endif

    #if defined(ROBOX_DEBUG_I2C)
        ESP_LOGI(LOG_MAIN_TAG, "IO expander write");
        io->set_output(0, 0x00);
        // io->set_output(1, 0x00);
        delay(100);
        io->set_output(0, 0x80);
        // io->set_output(1, 0xFF);
        delay(100);

        // ESP_LOGI(LOG_MAIN_TAG, "IO expander read");
        // uint8_t data = io->get_configure_outputs(1);
        // ESP_LOGI(LOG_MAIN_TAG, "IO expander read %2x", data);

    #endif

    // #if defined(ROBOX_DEBUG_I2C_SCANNER)
    //     scanner_loop();
    //     delay(10000);
    // #endif

    // #if defined(ROBOX_MOTOR)
    // if (timekeeper < millis()) {
    //     Serial.printf("new motor program. timekeeper: %ld, program %d\n", timekeeper, (motor_test_program % MOTOR_TEST_PROGRAMS));
    //     timekeeper += 5000;


    //     switch (motor_test_program % MOTOR_TEST_PROGRAMS)
    //     {
    //     case 0:
    //         motor->set_direction(0, 0);
    //         motor->set_speed(0.3, 0.3);
    //         break;

    //     case 1:
    //         motor->set_direction(1, 1);
    //         motor->set_speed(0.3, 0.3);
    //         break;
        
    //     case 2:
    //         motor->set_direction(1, 0);
    //         motor->set_speed(0.3, 0.3);
    //         break;

    //     case 3:
    //         motor->set_direction(0, 1);
    //         motor->set_speed(0.3, 0.3);
    //         break;

    //     // case 4:
    //     //     motor->set_direction(0, 1);
    //     //     motor->set_speed(0.3, 0.3);
    //     //     break;

    //     default:
    //         break;
    //     }

    //     motor_test_program++;
    // }
    // #endif

    // #if defined(ROBOX_IMPROV)
    //     improvSerial.handleSerial();
    // #endif

    // led_breath(true, r_blue);

    // #if defined(ROBOX_WIFI_MANAGER)
    // doWiFiManager();
    // #endif
    if (mux.get_current_source() == BleSource) {
        if (is_ble_on == false) {
            led_breath(true, r_blue);
        }
        else if ((is_ble_on = true) && (is_ble_connected == false)) {
            led_breath(false, r_blue);
        }
    }
    
}
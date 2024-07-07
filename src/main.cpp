#include <Arduino.h>
#include "AudioTools.h"

/*
 * Compile parts of the project:
 * - ROBOX_FULL: the complete audio project, with audio mux
 * - ROBOX_EXAMPLE_*: the examples, from the arduino audio library, with minimal adaptations
 * - ROBOX_COMPONENT_*: the examples, from the arduino audio library, with adaptations for the audio mux
 *
 * Select only one of the above
 */

// #define ROBOX_FULL

// #define ROBOX_EXAMPLE_BLE
#define ROBOX_EXAMPLE_BLE_BEAT
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
 *
 * Multiple can be selected
 */

// #define ROBOX_LCD
// #define ROBOX_BATTERY
// #define ROBOX_MOTOR
// #define ROBOX_TEST_ADC_KEY
// #define ROBOX_DEBUG_CLI
// #define ROBOX_DEBUG_I2C
// #define ROBOX_DEBUG_I2C_SCANNER

/*
 * compile options logic
 */

#if defined(ROBOX_FULL)
    #include "robox_audio_mux.h"

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

// #include "ble_example.h"
// #include "ble_copier.h"
// #include "streams_copy_example.h"
// #include "listing.h"
#include "general_definitions.h"
#include "wifi_credentials.h"

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
static RoboxMotor* motor;
unsigned long timekeeper = 0;
uint8_t motor_test_program = 0;
#define MOTOR_TEST_PROGRAMS 5
#endif

#if defined(ROBOX_BATTERY)
static RoboxBattery* battery;
#endif


void setup() {
    Serial.begin(115200);

    AudioLogger::instance().begin(Serial, AudioLogger::Info);
    
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

    #if defined(ROBOX_FULL)
        ESP_LOGI(LOG_MAIN_TAG, "Setup mux");
        mux.setup();
        // mux.switch_to(BleSource);
        // mux.switch_to(WebRadioSource);
        // mux.switch_to(SDSource);

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
    motor->set_direction(0, 0);
    motor->set_speed(1.0, 1.0);
    motor->enable(1);

    timekeeper = millis() + 5000;
    #endif

}

void loop() {
    #if defined(ROBOX_FULL)
        mux.copy();
    
    #elif defined(ROBOX_EXAMPLE_BLE) || defined(ROBOX_EXAMPLE_BLE_BEAT) || defined(ROBOX_EXAMPLE_SD) || defined(ROBOX_EXAMPLE_WEB) || defined(ROBOX_EXAMPLE_MULTI_WEB_FFT) || defined(ROBOX_EXAMPLE_WEB_PLAYER_BEAT) || defined(ROBOX_EXAMPLE_SD_PLAYER_BEAT)
        player_loop();

    #elif defined(ROBOX_COMPONENT_BLE)
        ble.mux_copy();

    #elif defined(ROBOX_COMPONENT_WEB)
        web.mux_copy();

    #elif defined(ROBOX_COMPONENT_SD)
        sd.mux_copy();

    #endif


    #if defined(ROBOX_LCD)
        ESP_LOGI(LOG_MAIN_TAG, "LCD loop");

        screen->lcd_gfx_test();
    #endif

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

    #if defined(ROBOX_DEBUG_I2C_SCANNER)
        scanner_loop();
        delay(10000);
    #endif

    #if defined(ROBOX_MOTOR)
    if (timekeeper < millis()) {
        Serial.printf("new motor program. timekeeper: %ld, program %d\n", timekeeper, (motor_test_program % MOTOR_TEST_PROGRAMS));
        timekeeper += 5000;


        switch (motor_test_program % MOTOR_TEST_PROGRAMS)
        {
        case 0:
            motor->set_direction(0, 0);
            motor->set_speed(1.0, 1.0);
            break;

        case 1:
            motor->set_direction(1, 1);
            motor->set_speed(1.0, 1.0);
            break;
        
        case 2:
            motor->set_direction(1, 0);
            motor->set_speed(1.0, 1.0);
            break;

        case 3:
            motor->set_direction(0, 1);
            motor->set_speed(1.0, 1.0);
            break;

        case 4:
            motor->set_direction(0, 1);
            motor->set_speed(0.9, 0.1);
            break;

        default:
            break;
        }

        motor_test_program++;
    }
    #endif
}
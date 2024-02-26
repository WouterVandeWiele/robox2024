#include <Arduino.h>
// #include "lcd_config.h"
// #include "debug_cli.h"
// #include "audio_mux.h"
// #include "ble.h"
// #include "web_radio.h"
#include "web_example_meta.h"
#include "general_definitions.h"

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

// RoboxAudioMux mux;
// RoboxBluetooth ble;
// RoboxWebRadio web;

void setup() {
    Serial.begin(115200);
    
    // setup logging
    esp_log_level_set("*", ESP_LOG_WARN);
    // esp_log_level_set(BT_AV_TAG, ESP_LOG_NONE);
    // esp_log_level_set(BT_APP_TAG, ESP_LOG_NONE);
    esp_log_level_set(LOG_MAIN_TAG, ESP_LOG_DEBUG);
    esp_log_level_set(LOG_BLE_TAG, ESP_LOG_DEBUG);
    esp_log_level_set(LOG_MUX_TAG, ESP_LOG_DEBUG);
    esp_log_level_set(LOG_I2S_TAG, ESP_LOG_DEBUG);
    esp_log_level_set(LOG_SD_TAG, ESP_LOG_DEBUG);
    esp_log_level_set(LOG_WEB_TAG, ESP_LOG_DEBUG);

    ESP_LOGI(LOG_MAIN_TAG, "Setup mux");
    // mux.setup();
    // ble.setup();
    // web.setup();

    // ESP_LOGI(LOG_TAG, "lcd setup");
    // lcd_setup();
    // ESP_LOGI(LOG_TAG, "i2s setup");
    // i2s_setup();

    ESP_LOGI(LOG_MAIN_TAG, "select ble source");
    // mux.switch_to(BleSource);
    // debug_cli_setup();
    // audio_setup();
    // ble.mux_start();
    web_setup();
}

void loop() {
    // ble.mux_copy();
    // web.mux_copy();
    // lcd_test();
    // debug_cli_loop();
    // delay(100);

    // mux.copy();
    web_loop();
}
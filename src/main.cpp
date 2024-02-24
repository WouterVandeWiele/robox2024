#include <Arduino.h>
// #include "lcd_config.h"
// #include "debug_cli.h"
// #include "audio_mux.h"
#include "ble.h"
// #define USE_AUDIO_LOGGING true

#define LOG_TAG "Robox main"

// RoboxAudioMux mux;
RoboxBluetooth ble;

void setup() {
    Serial.begin(115200);

    ESP_LOGI(LOG_TAG, "Setup mux");
    ble.setup();
    
    // mux = RoboxAudioMux();

    // ESP_LOGI(LOG_TAG, "lcd setup");
    // lcd_setup();
    // ESP_LOGI(LOG_TAG, "i2s setup");
    // i2s_setup();

    ESP_LOGI(LOG_TAG, "select ble source");
    // mux.switch_to(BleSource);
    // debug_cli_setup();
    // audio_setup();
    // ble.mux_start();
}

void loop() {
    // ble.mux_copy();
    // lcd_test();
    // debug_cli_loop();
    // delay(100);

    // mux.copy();
}
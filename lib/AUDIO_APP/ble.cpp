#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include "ble.h"
#include "i2s.h"

#define LOG_TAG "Robox BLE"


// callback used by A2DP to provide the sound data
void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
  Serial.printf("> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);
}


// RoboxBluetooth::RoboxBluetooth(): a2dp_sink() {
//     ESP_LOGI(LOG_TAG, ">>> BLE I2S starting...");
//     // AudioLogger::instance().begin(Serial, AudioLogger::Warning);

//     // register data callback
//     a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
//     a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM | ESP_AVRC_MD_ATTR_TRACK_NUM | ESP_AVRC_MD_ATTR_NUM_TRACKS | ESP_AVRC_MD_ATTR_GENRE | ESP_AVRC_MD_ATTR_PLAYING_TIME);
    
//     ESP_LOGI(LOG_TAG, "set i2s pins");
//     a2dp_sink.set_pin_config(my_i2s_pin_config);

//     // Start Bluetooth Audio Receiver
//     ESP_LOGI(LOG_TAG, "set i2s config");
//     a2dp_sink.set_i2s_config(my_i2s_config);
//     ESP_LOGI(LOG_TAG, "set reconnect");
//     a2dp_sink.set_auto_reconnect(true);
//     // a2dp_sink.start(A2DP_BLE_NAME);
//     // delay(10);
//     ESP_LOGI(LOG_TAG, "start sink");
//     a2dp_sink.start("a2dp-i2s");    // watchdog gets triggered here
//     // a2dp_sink.start()
//     // delay(10);

//     ESP_LOGI(LOG_TAG, "<<< ble setup completed");
// }


void RoboxBluetooth::setup() {
    ESP_LOGI(LOG_TAG, ">>> BLE I2S starting...");
    // a2dp_sink = ;
    // AudioLogger::instance().begin(Serial, AudioLogger::Warning);

    // register data callback
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM | ESP_AVRC_MD_ATTR_TRACK_NUM | ESP_AVRC_MD_ATTR_NUM_TRACKS | ESP_AVRC_MD_ATTR_GENRE | ESP_AVRC_MD_ATTR_PLAYING_TIME);
    
    ESP_LOGI(LOG_TAG, "set i2s pins");
    a2dp_sink.set_pin_config(my_i2s_pin_config);

    // Start Bluetooth Audio Receiver
    ESP_LOGI(LOG_TAG, "set i2s config");
    a2dp_sink.set_i2s_config(my_i2s_config);
    ESP_LOGI(LOG_TAG, "set reconnect");
    a2dp_sink.set_auto_reconnect(true);
    // a2dp_sink.start(A2DP_BLE_NAME);
    // delay(10);
    ESP_LOGI(LOG_TAG, "start sink");
    a2dp_sink.start("a2dp-i2s");    // watchdog gets triggered here
    // a2dp_sink.start()
    // delay(10);

    ESP_LOGI(LOG_TAG, "<<< ble setup completed");
}

/*
BLE Audio MUX controls
*/

void RoboxBluetooth::mux_stop() {
    a2dp_sink.set_stream_reader(nullptr, false);  // keep bluetooth on, just disable stream copy to i2s
    // a2dp_sink.stop();
}

void RoboxBluetooth::mux_start() {
    a2dp_sink.set_stream_reader(nullptr, true);
    // ble_setup();
}




/**
 * @file basic-a2dp-audioi2s.ino
 * @brief A2DP Sink with output to I2SStream. This example is of small value
 * since my Bluetooth Library already provides I2S output out of the box.
 *
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

#include "BluetoothA2DPSink.h"
#include "AudioTools.h"

#include "i2s.h"
#include "general_definitions.h"

BluetoothA2DPSink a2dp_sink;
// I2SStream i2s;

// callback used by A2DP to provide the sound data
void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
  Serial.printf("> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);
}

// Write data to I2S
void read_data_stream(const uint8_t *data, uint32_t length) {
  i2s.write(data, length);
}

void player_setup() {
    ESP_LOGI(LOG_BLE_TAG, ">>> BLE I2S starting...");
    // AudioLogger::instance().begin(Serial, AudioLogger::Warning);

    // register data callback
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    
    ESP_LOGI(LOG_BLE_TAG, "set i2s");
    a2dp_sink.set_pin_config(my_i2s_pin_config);
    a2dp_sink.set_i2s_config(my_i2s_config);


    a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM | ESP_AVRC_MD_ATTR_TRACK_NUM | ESP_AVRC_MD_ATTR_NUM_TRACKS | ESP_AVRC_MD_ATTR_GENRE | ESP_AVRC_MD_ATTR_PLAYING_TIME);
    
    // ESP_LOGI(LOG_BLE_TAG, "set reconnect");
    // a2dp_sink.set_auto_reconnect(true);
    a2dp_sink.set_auto_reconnect(true);

    ESP_LOGI(LOG_BLE_TAG, "start sink");
    a2dp_sink.start("a2dp-i2s");

    // i2s = I2s(I2S_PIN_MUTE)


    ESP_LOGI(LOG_BLE_TAG, "<<< BLE setup completed");
}

void player_loop() { delay(100); }
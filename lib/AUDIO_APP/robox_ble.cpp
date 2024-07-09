#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include "robox_audio_mux.h"
#include "robox_ble.h"
#include "robox_i2s.h"
#include "general_definitions.h"


extern RoboxAudioMux mux;


// callback used by A2DP to provide the sound data
void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
  Serial.printf("> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);

  if (id == 0x1) {
    // const std::lock_guard<std::mutex> lock(mux.meta_mutex);
    mux.meta.title = String((char*) text);
  }
}

void RoboxBluetooth::mux_start() {
    ESP_LOGI(LOG_BLE_TAG, ">>> BLE I2S starting...");

    
    ESP_LOGI(LOG_BLE_TAG, "set i2s");
    a2dp_sink.set_pin_config(my_i2s_pin_config);
    a2dp_sink.set_i2s_config(my_i2s_config);

    ESP_LOGI(LOG_BLE_TAG, "register data callback");
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM | ESP_AVRC_MD_ATTR_TRACK_NUM | ESP_AVRC_MD_ATTR_NUM_TRACKS | ESP_AVRC_MD_ATTR_GENRE | ESP_AVRC_MD_ATTR_PLAYING_TIME);
    
    ESP_LOGI(LOG_BLE_TAG, "set reconnect");
    a2dp_sink.set_auto_reconnect(true);

    ESP_LOGI(LOG_BLE_TAG, "start sink");
    a2dp_sink.start("a2dp-i2s");

    ESP_LOGI(LOG_BLE_TAG, "<<< BLE setup completed");
}

/*
BLE Audio MUX controls
*/

void RoboxBluetooth::mux_stop() {
  ESP_LOGI(LOG_BLE_TAG, "<<< BLE stopping");
  a2dp_sink.end(true);

  ESP_LOGI(LOG_BLE_TAG, "<<< BLE stopped");
}

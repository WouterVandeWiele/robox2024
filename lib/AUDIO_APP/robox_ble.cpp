#include <Arduino.h>
#include "AudioTools.h"
#include "AudioLibs/AudioRealFFT.h"
#include "BluetoothA2DPSink.h"
#include "robox_audio_mux.h"
#include "robox_ble.h"
#include "robox_i2s.h"
#include "general_definitions.h"
#include "robox_fft_beat.h"
#include "robox_restart.h"
#include "lcd_screen.h"

extern RoboxAudioMux mux;
extern RoboxRestartManager restart_manager;
static esp_avrc_playback_stat_t playback_status;
// extern AudioRealFFT fft;

// audio stream + LED beat detection
static void write_stream_beat_led(const uint8_t *data, uint32_t length) {
  i2s.write(data, length);
  fft.write(data, length);
}

// only audio stream
static void write_stream(const uint8_t *data, uint32_t length) {
  i2s.write(data, length);
}

void avrc_rn_playstatus_callback(esp_avrc_playback_stat_t playback) {
    playback_status = playback;
}


// callback used by A2DP to provide the sound data
void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
    Serial.printf("> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);
    std::lock_guard<std::mutex> lck(meta_data_mtx);

    switch (id)
    {
    case ESP_AVRC_MD_ATTR_TITLE:
        mux.meta.title = String((char*) text);
        lcd_invalidate(INVALIDATE_ALL);
        break;

    case ESP_AVRC_MD_ATTR_ARTIST:
        break;

    case ESP_AVRC_MD_ATTR_ALBUM:
        break;
    
    case ESP_AVRC_MD_ATTR_TRACK_NUM:
        break;

    case ESP_AVRC_MD_ATTR_NUM_TRACKS:
        break;

    case ESP_AVRC_MD_ATTR_GENRE:
        break;

    case ESP_AVRC_MD_ATTR_PLAYING_TIME:
        break;
    
    default:
        break;
    }

}

void RoboxBluetooth::mux_start() {
    ESP_LOGI(LOG_BLE_TAG, ">>> BLE I2S starting...");

    
    ESP_LOGI(LOG_BLE_TAG, "set i2s");
    // a2dp_sink.set_pin_config(my_i2s_pin_config);
    // a2dp_sink.set_i2s_config(my_i2s_config);
    

    ESP_LOGI(LOG_BLE_TAG, "register data callback");
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    a2dp_sink.set_avrc_metadata_attribute_mask(
          ESP_AVRC_MD_ATTR_TITLE 
        | ESP_AVRC_MD_ATTR_ARTIST 
        | ESP_AVRC_MD_ATTR_ALBUM 
        | ESP_AVRC_MD_ATTR_TRACK_NUM 
        | ESP_AVRC_MD_ATTR_NUM_TRACKS 
        | ESP_AVRC_MD_ATTR_GENRE 
        | ESP_AVRC_MD_ATTR_PLAYING_TIME
    );

    playback_status = ESP_AVRC_PLAYBACK_ERROR;
    a2dp_sink.set_avrc_rn_playstatus_callback(avrc_rn_playstatus_callback);


    if (beat_led) {
        a2dp_sink.set_stream_reader(write_stream_beat_led, false);
    }
    else {
        a2dp_sink.set_stream_reader(write_stream, false);
    }
    
    ESP_LOGI(LOG_BLE_TAG, "set reconnect");
    a2dp_sink.set_auto_reconnect(true);

    String ssid = restart_manager.getDefaultName();

    Serial.printf("ble name: %s\n", ssid.c_str());

    ESP_LOGI(LOG_BLE_TAG, "start sink");
    a2dp_sink.start(ssid.c_str());


    i2s_setup();
    if (beat_led) {
        fft_beat_setup(a2dp_sink.sample_rate());
    }
    ESP_LOGI(LOG_BLE_TAG, "<<< BLE setup completed");
}

/*
BLE Audio MUX controls
*/

void RoboxBluetooth::mux_stop() {
    ESP_LOGI(LOG_BLE_TAG, "<<< BLE stopping");
    a2dp_sink.set_volume(0);
    a2dp_sink.stop();
    a2dp_sink.end(true);

    i2s_driver_uninstall((i2s_port_t)0);

    btStop();

    ESP_LOGI(LOG_BLE_TAG, "<<< BLE stopped");
}

void RoboxBluetooth::volume(float level) {
    if (volume_level > 1.0) {
        volume_level = 1.0;
    }
    else if (volume_level < 0.0) {
        volume_level = 0.0;
    }

    a2dp_sink.set_volume((uint8_t)(0x7f*level));
}

bool RoboxBluetooth::audio_active() {
    bool active = false;
    
    if ((playback_status == ESP_AVRC_PLAYBACK_PLAYING) || 
        (playback_status == ESP_AVRC_PLAYBACK_FWD_SEEK) || 
        (playback_status == ESP_AVRC_PLAYBACK_REV_SEEK)) {
        active = true;
    }

    return active;
}

void RoboxBluetooth::audio_play() {
    is_audio_paused = false;
    // digitalWrite(I2S_PIN_MUTE, HIGH);
    // delay(300);
    a2dp_sink.play();
}

void RoboxBluetooth::audio_pause() {
    is_audio_paused = true;
    // digitalWrite(I2S_PIN_MUTE, LOW);
    // delay(300);
    a2dp_sink.pause();
}

void RoboxBluetooth::audio_next() {
  audio_pause();
  a2dp_sink.next();
  audio_play();
}

void RoboxBluetooth::audio_previous() {
  audio_pause();
  a2dp_sink.previous();
  audio_play();
}
#include "audio_mux.h"
#include "i2s.h"
#include "general_definitions.h"

const char* audio_source_names[] = {"NotSelected", "Ble", "WebRadio", "SD"};

// RoboxAudioMux::RoboxAudioMux(): ble(), web(), sd(), current_source(nullptr) {
// RoboxAudioMux::RoboxAudioMux():  {

// }

void RoboxAudioMux::setup() {
    ESP_LOGI(LOG_MUX_TAG, ">>> Audio Mux starting...");
    ESP_LOGD(LOG_MUX_TAG, "setup BLE");
    ble.setup();

    ESP_LOGD(LOG_MUX_TAG, "init i2s");
    // i2s_cfg = i2s.defaultConfig(TX_MODE);
    // i2s_cfg.sample_rate = my_i2s_config.sample_rate;
    // i2s_cfg.bits_per_sample = my_i2s_config.bits_per_sample;
    // i2s_cfg.channel_format = my_i2s_config.channel_format;
    // // i2s_cfg.channel_format = my_i2s_config.communication_format;
    
    // i2s_cfg.buffer_count = my_i2s_config.dma_buf_count;
    // i2s_cfg.buffer_size = my_i2s_config.dma_buf_len;
    // i2s_cfg.use_apll = my_i2s_config.use_apll;
    // i2s_cfg.auto_clear = my_i2s_config.tx_desc_auto_clear;
    // i2s_cfg.pin_bck = my_i2s_pin_config.bck_io_num;
    // i2s_cfg.pin_ws = my_i2s_pin_config.ws_io_num;
    // i2s_cfg.pin_data = my_i2s_pin_config.data_out_num;
    
    // i2s.begin(i2s_cfg);

    ESP_LOGD(LOG_MUX_TAG, "setup Web");
    // web.setup();

    ESP_LOGD(LOG_MUX_TAG, "setup SD");
    sd.setup();

    ESP_LOGI(LOG_BLE_TAG, "<<< Audio Mux setup completed");
}


void RoboxAudioMux::switch_to(audio_source new_mux_source) {
    if (current_source != nullptr) {
        current_source->mux_stop();
    }

    switch (new_mux_source)
    {
    case NotSelectedSource:
        current_source = nullptr;
        break;

    case BleSource:
        current_source = &ble;
        break;

    case WebRadioSource:
        // current_source = &web;
        break;

    case SDSource:
        current_source = &sd;
        break;

    default:
        // Serial.printf("Unsupported audio type, %s", audio_source_names[new_mux_source]);
        ESP_LOGW(LOG_MUX_TAG, "Unsupported audio type %s", audio_source_names[new_mux_source]);
        break;
    }

    if (current_source != nullptr) {
        current_source->mux_start();
    }
}

void RoboxAudioMux::copy() {
    if (current_source != nullptr) {
        current_source->mux_copy();
    }
}
#include "robox_audio_mux.h"
#include "robox_i2s.h"
#include "general_definitions.h"

#include "robox_ble.h"
#include "robox_web.h"
#include "robox_sd.h"

const char* audio_source_names[] = {"NotSelected", "Ble", "WebRadio", "SD"};

void RoboxAudioMux::setup() {
    ESP_LOGI(LOG_MUX_TAG, ">>> Audio Mux starting...");

    ESP_LOGI(LOG_BLE_TAG, "<<< Audio Mux setup completed");
}


void RoboxAudioMux::switch_to(audio_source new_mux_source) {
    ESP_LOGI(LOG_MUX_TAG, "Audio Mux switch from: %s", audio_source_names[source_name]);

    if (source_name != NotSelectedSource) {
        current_source.mux_stop();
    }

    ESP_LOGI(LOG_MUX_TAG, "Audio Mux switching to: %s", audio_source_names[new_mux_source]);

    source_name = new_mux_source;
    switch (new_mux_source)
    {
    case NotSelectedSource:
        break;

    case BleSource:
        current_source = RoboxBluetooth();
        break;

    case WebRadioSource:
        current_source = RoboxWebRadio();
        break;

    case SDSource:
        current_source = RoboxSD();
        break;

    default:
        ESP_LOGW(LOG_MUX_TAG, "Unsupported audio type %s", audio_source_names[new_mux_source]);
        break;
    }

    ESP_LOGI(LOG_MUX_TAG, "Audio Mux init new driver");

    if (source_name != NotSelectedSource) {
        current_source.mux_start();
    }

    ESP_LOGI(LOG_MUX_TAG, "Audio Mux switch complete");
}

void RoboxAudioMux::copy() {
    if (source_name != NotSelectedSource) {
        current_source.mux_copy();
    }
}
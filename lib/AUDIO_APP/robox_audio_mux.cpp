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
    Serial.printf("Audio Mux switch from: %s\n", audio_source_names[source_name]);

    if (source_name != new_mux_source) {
        if (source_name != NotSelectedSource) {
            current_source->mux_stop();
            // current_source->~MuxInterface();
        }

        delay(100);

        Serial.printf("Audio Mux switching to: %s\n", audio_source_names[new_mux_source]);

        source_name = NotSelectedSource;
        
        Serial.printf("Audio Mux init new driver: %s\n", audio_source_names[new_mux_source]);

        meta.title = "";
        switch (new_mux_source)
        {
        case NotSelectedSource:
            current_source.reset();
            break;

        case BleSource:
            current_source.reset(new RoboxBluetooth(true, volume_level));
            current_source->mux_start();
            break;

        case WebRadioSource:
            current_source.reset(new RoboxWebRadio(true, volume_level));
            current_source->mux_start();
            break;

        case SDSource:
            current_source.reset(new RoboxSD(true, volume_level));
            current_source->mux_start();
            break;

        default:
            Serial.printf("Unsupported audio type %s\n", audio_source_names[new_mux_source]);
            break;
        }

        delay(100);

        source_name = new_mux_source;
        Serial.printf("Audio Mux switch complete\n");
    }
}

void RoboxAudioMux::copy() {
    if (source_name != NotSelectedSource) {
        current_source->mux_copy();
    }
}

void RoboxAudioMux::volume(float level) {
    current_source->volume(level);
}

void RoboxAudioMux::volume_increment() {
    current_source->volume_increment();
}

void RoboxAudioMux::volume_decrement() {
    current_source->volume_decrement();
}

void RoboxAudioMux::audio_play_pause() {
    current_source->audio_play_pause();
}
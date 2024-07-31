#include <WiFiManager.h>
#include "robox_audio_mux.h"
#include "robox_i2s.h"
#include "general_definitions.h"

#include <GEM_adafruit_gfx.h>

extern GEM_adafruit_gfx* menu;

#include "robox_ble.h"
#include "robox_web.h"
#include "robox_sd.h"
#include "robox_void.h"

#if defined(ROBOX_WIFI_MANAGER)
extern WiFiManager wifiManager;
#endif

const char* audio_source_names[] = {"NotSelected", "Ble", "WebRadio", "SD"};
bool is_audio_paused = false;
std::mutex meta_data_mtx;

void RoboxAudioMux::setup() {
    ESP_LOGI(LOG_MUX_TAG, ">>> Audio Mux starting...");
    pinMode(I2S_PIN_MUTE, OUTPUT);
    current_source.reset(new RoboxVoid(false, volume_level));
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

        #if defined(ROBOX_WIFI_MANAGER)
        
        if (source_name == BleSource) {
            WiFi.disconnect(true, false);
            WiFi.mode(WIFI_OFF);
        }
        else if (WiFi.status() != WL_CONNECTED) {
            uint64_t _chipmacid = 0LL;
            esp_efuse_mac_get_default((uint8_t*) (&_chipmacid));
            String hostString = String((uint32_t)_chipmacid, HEX);
            hostString.toUpperCase();
            String ssid = "ROBOX_" + hostString;

            wifiManager.autoConnect(ssid.c_str(), NULL);
        }
        #endif

        Serial.printf("Audio Mux switching to: %s\n", audio_source_names[new_mux_source]);

        source_name = NotSelectedSource;
        is_audio_paused = false;
        digitalWrite(I2S_PIN_MUTE, HIGH);
        
        Serial.printf("Audio Mux init new driver: %s\n", audio_source_names[new_mux_source]);

        meta.title = "";
        switch (new_mux_source)
        {
        case NotSelectedSource:
            current_source.reset(new RoboxVoid(false, volume_level));
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
    if ((source_name != NotSelectedSource) && (is_audio_paused == false)) {
        current_source->mux_copy();
    }
}

void RoboxAudioMux::volume(float level) {
    if (source_name != NotSelectedSource) {
        current_source->volume(level);
        if (menu->context.loop != nullptr) {
            menu->registerKeyPress(GEM_KEY_NONE);
        }
    }
}

float RoboxAudioMux::get_volume() {
    if (source_name != NotSelectedSource) {
        return current_source->get_volume();
    }
    else {
        return 0.0;
    }
}

void RoboxAudioMux::volume_increment() {
    current_source->volume_increment();
}

void RoboxAudioMux::volume_decrement() {
    current_source->volume_decrement();
}

void RoboxAudioMux::audio_play_pause() {
    if (source_name != NotSelectedSource) {
        current_source->audio_play_pause();
    }
}

void RoboxAudioMux::audio_next() {
    if (source_name != NotSelectedSource) {
        current_source->audio_next();
    }
}

void RoboxAudioMux::audio_previous() {
    if (source_name != NotSelectedSource) {
        current_source->audio_previous();
    }
}

bool RoboxAudioMux::audio_active() {
    if (source_name != NotSelectedSource) {
        return current_source->audio_active();
    }
    else {
        return false;
    }
}
#include <WiFiManager.h>
#include "robox_audio_mux.h"
#include "robox_i2s.h"
#include "general_definitions.h"
#include "robox_restart.h"

#include <GEM_adafruit_gfx.h>

extern GEM_adafruit_gfx* menu;

#include "robox_ble.h"
#include "robox_web.h"
#include "robox_sd.h"
#include "robox_void.h"

// extern WiFiManager wifiManager;
extern RoboxRestartManager restart_manager;

const char* audio_source_names[] = {"NotSelected", "Ble", "WebRadio", "SD"};
bool is_audio_paused = false;
// static bool is_startup = true;
std::mutex meta_data_mtx;

void RoboxAudioMux::setup() {
    ESP_LOGI(LOG_MUX_TAG, ">>> Audio Mux starting...");
    pinMode(I2S_PIN_MUTE, OUTPUT);

    if (restart_manager.is_cold_boot()) {
        current_source.reset(new RoboxVoid(false, volume_level));
    }
    else {
        source_name = restart_manager.get_startup_source();
        switch_startup();
    }

    ESP_LOGI(LOG_BLE_TAG, "<<< Audio Mux setup completed");
}

void RoboxAudioMux::switch_to(audio_source new_mux_source) {
    Serial.printf("Audio Mux switch from: %s\n", audio_source_names[source_name]);

    if (source_name != new_mux_source) {
        current_source->mux_stop();
        delay(100);
        restart_manager.reboot_next_source(new_mux_source);
    }
    else {
        Serial.printf("Same source, not switching\n");
    }
}

void RoboxAudioMux::switch_startup() {
    Serial.printf("Audio Mux switching to: %s\n", audio_source_names[source_name]);

    // source_name = NotSelectedSource;
    digitalWrite(I2S_PIN_MUTE, HIGH);
    
    Serial.printf("Audio Mux init new driver: %s\n", audio_source_names[source_name]);

    meta.title = "";
    switch (source_name)
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
        Serial.printf("Unsupported audio type %s\n", audio_source_names[source_name]);
        break;
    }

    delay(100);

    is_startup = false;
    // is_audio_paused = false;
    // source_name = new_mux_source;
    Serial.printf("Audio Mux switch complete\n");
}

void RoboxAudioMux::copy() {
    if ((source_name != NotSelectedSource) && (is_audio_paused == false) && (is_startup == false)) {
        current_source->mux_copy();
    }
}

void RoboxAudioMux::volume(float level) {
    if ((source_name != NotSelectedSource) && (is_startup == false)) {
        current_source->volume(level);
        if (menu->context.loop != nullptr) {
            menu->registerKeyPress(GEM_KEY_NONE);
        }
    }
}

float RoboxAudioMux::get_volume() {
    if (is_startup == false) {
        return current_source->get_volume();
    }
    else {
        return 0.0;
    }
}

void RoboxAudioMux::volume_increment() {
    if (is_startup == false) {
        current_source->volume_increment();
    }
}

void RoboxAudioMux::volume_decrement() {
    if (is_startup == false) {
        current_source->volume_decrement();
    }
}

void RoboxAudioMux::audio_play_pause() {
    if ((source_name != NotSelectedSource) && (is_startup == false)) {
        current_source->audio_play_pause();
    }
}

void RoboxAudioMux::audio_next() {
    if ((source_name != NotSelectedSource) && (is_startup == false)) {
        current_source->audio_next();
    }
}

void RoboxAudioMux::audio_previous() {
    if ((source_name != NotSelectedSource) && (is_startup == false)) {
        current_source->audio_previous();
    }
}

bool RoboxAudioMux::audio_active() {
    if ((source_name != NotSelectedSource) && (is_startup == false)) {
        return current_source->audio_active();
    }
    else {
        return false;
    }
}

String RoboxAudioMux::get_title() {
    std::lock_guard<std::mutex> lck(meta_data_mtx);

    return meta.title;
}
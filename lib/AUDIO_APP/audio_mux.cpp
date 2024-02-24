#include "audio_mux.h"

// RoboxAudioMux::RoboxAudioMux(): ble(), web(), sd(), current_source(nullptr) {
RoboxAudioMux::RoboxAudioMux(): current_source(nullptr) {

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
        current_source = nullptr;
        // current_source = &ble;
        break;

    case WebRadioSource:
        current_source = nullptr;
        // current_source = &web;
        break;

    case SDSource:
        current_source = nullptr;
        // current_source = &sd;
        break;

    default:
        // Serial.printf("Unsupported audio type, %s", audio_source_names[new_mux_source]);
        Serial.printf("Unsupported audio type");
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
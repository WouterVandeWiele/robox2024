#ifndef AUDIO_MUX_H
#define AUDIO_MUX_H
/**
 * @file basic-a2dp-audioi2s.ino
 * @brief A2DP Sink with output to I2SStream. This example is of small value
 * since my Bluetooth Library already provides I2S output out of the box.
 *
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
// robox audio architecture overview [docs/audio_overview.excalidraw.png]
#include "AudioTools.h"
#include "mux_interface.h"
#include "ble.h"
#include "web_radio.h"
#include "sd.h"


enum audio_source {NotSelectedSource, BleSource, WebRadioSource, SDSource};
// const char* audio_source_names[] = {"NotSelected", "Ble", "WebRadio", "SD"};

class RoboxAudioMux {
    public:
        RoboxAudioMux();

        void switch_to(audio_source new_mux_source);

        void copy();

    private:
        RoboxBluetooth ble;
        RoboxWebRadio web;
        RoboxSD sd;
        MuxInterface* current_source;
};


#endif  // AUDIO_MUX_H
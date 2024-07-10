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
// #include "AudioTools.h"
#include <mutex>
#include <Arduino.h>
#include "robox_mux_interface.h"
#include "general_definitions.h"



enum audio_source {NotSelectedSource, BleSource, WebRadioSource, SDSource};
extern const char* audio_source_names[];

typedef struct MetaData{
  String title;
} MetaData;

class RoboxAudioMux {
    public:
        RoboxAudioMux(): meta(), source_name(NotSelectedSource) {};

        void setup();

        void switch_to(audio_source new_mux_source);

        void copy();

        void volume(float level);

        MetaData meta;
        // std::mutex meta_mutex;
    private:

        audio_source source_name;
        MuxInterface* current_source;

};


#endif  // AUDIO_MUX_H
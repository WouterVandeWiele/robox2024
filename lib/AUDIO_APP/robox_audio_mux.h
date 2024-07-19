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
#include <memory>
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
        RoboxAudioMux()
          : meta()
          , source_name(NotSelectedSource) 
          , volume_level(0.2)
        {};

        void setup();

        void switch_to(audio_source new_mux_source);

        void copy();

        void volume(float level);
        void volume_increment();
        void volume_decrement();

        MetaData meta;
        // std::mutex meta_mutex;
    private:

        volatile audio_source source_name;
        std::unique_ptr<MuxInterface> current_source;
        float volume_level;

};


#endif  // AUDIO_MUX_H
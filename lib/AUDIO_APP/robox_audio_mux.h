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

extern bool is_audio_paused;

extern std::mutex meta_data_mtx;
typedef struct MetaData{
  String title;         // serves now as text buffer for the LCD
  uint32_t play_time;

} MetaData;

class RoboxAudioMux {
    public:
        RoboxAudioMux()
          : meta()
          , source_name(NotSelectedSource) 
          , volume_level(0.2)
          , is_startup(true)
        {};

        void setup();

        void switch_to(audio_source new_mux_source);
        void switch_startup();

        void copy();

        void volume(float level);
        float get_volume();
        void volume_increment();
        void volume_decrement();

        void audio_play_pause();
        void audio_next();
        void audio_previous();

        String get_title();

        audio_source get_current_source();

        // std::mutex meta_mutex;

        bool audio_active();
        MetaData meta;
    private:

        volatile audio_source source_name;
        std::unique_ptr<MuxInterface> current_source;
        float volume_level;

        bool is_startup;

};


#endif  // AUDIO_MUX_H
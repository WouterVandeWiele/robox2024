#ifndef SD_H
#define SD_H

#include "AudioTools.h"
#include "robox_mux_interface.h"

#include "AudioLibs/AudioSourceSDFAT.h"
#include "AudioCodecs/CodecMP3Helix.h"

#include "robox_i2s.h"

/*
SD Card Audio MUX controls
*/

class RoboxSD: public MuxInterface {
    public:
        RoboxSD(bool beat_led = false)
            : beat_led(beat_led)
            // : startFilePath("/")
            // , ext("mp3")
            // , source(startFilePath, ext)
            // , decoder()
            // , player(source, i2s, decoder) 
        {

        };
        /*
        SD Audio MUX controls
        */

        void mux_start() override;
        void mux_stop() override;
        void mux_copy() override;
        void volume(float level) override;

    private:
    bool beat_led;
        // const char *startFilePath;
        // const char* ext;
        // AudioSourceSDFAT source;
        // MP3DecoderHelix decoder;
        // AudioPlayer player;
};


#endif  // SD_H
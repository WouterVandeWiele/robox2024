#ifndef SD_H
#define SD_H

#include "AudioTools.h"
#include "robox_mux_interface.h"

#include "AudioLibs/AudioSourceSD.h"
#include "AudioCodecs/CodecMP3Helix.h"

#include "robox_i2s.h"

/*
SD Card Audio MUX controls
*/

class RoboxSD: public MuxInterface {
    public:
        RoboxSD(bool beat_led, float &volume_level)
            : MuxInterface(volume_level)
            , beat_led(beat_led)
            , startFilePath("/")
            , ext("mp3")
            , multi_output(i2s)
            , decoder()
            , source(startFilePath, ext)
            , player(source, multi_output, decoder) 
        {

        };
        /*
        SD Audio MUX controls
        */

        void mux_start() override;
        void mux_stop() override;
        void mux_copy() override;

        void volume(float level) override;

        virtual bool audio_active() override;
        virtual void audio_play() override;
        virtual void audio_pause() override;

    private:
    bool beat_led;
        const char *startFilePath;
        const char* ext;
        MultiOutput multi_output;
        MP3DecoderHelix decoder;
        AudioSourceSD source;
        AudioPlayer player;
};


#endif  // SD_H
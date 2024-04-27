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
        RoboxSD(): 
            startFilePath("/"),
            ext("mp3"),
            source(startFilePath, ext),
            player(source, i2s, decoder) {

        };
        /*
        SD Audio MUX controls
        */

        void mux_start();
        void mux_stop();
        void mux_copy();

    private:
        const char *startFilePath;
        const char* ext;
        AudioSourceSDFAT source;
        MP3DecoderHelix decoder;
        AudioPlayer player;
};


#endif  // SD_H
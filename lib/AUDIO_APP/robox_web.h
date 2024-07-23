#ifndef WEB_RADIO_H
#define WEB_RADIO_H

#include "AudioTools.h"
#include "robox_mux_interface.h"
#include "AudioCodecs/CodecMP3Helix.h"
// #include "audio_mux.h"
#include "wifi_credentials.h"
#include "general_definitions.h"

#include "robox_i2s.h"


// extern std::map<std::string, std::string> station_list;

// static const char *urls[] = {
//     "http://icecast.vrtcdn.be/mnm_hits-high.mp3"
// };

static const char *urls[] = {
    "http://icecast.vrtcdn.be/mnm_hits-high.mp3",
    "http://icecast.vrtcdn.be/mnm-high.mp3",
    "http://icecast.vrtcdn.be/radio1-high.mp3",
    "http://icecast.vrtcdn.be/ra2ant-high.mp3",
    "http://icecast.vrtcdn.be/stubru-high.mp3",
    // "http://streams.radio.dpgmedia.cloud/redirect/qmusic_be/mp3"
};


/*
Web radio Audio MUX controls
*/

class RoboxWebRadio: public MuxInterface {
    public:
        RoboxWebRadio(bool beat_led, float &volume_level)
            : MuxInterface(volume_level)
            , beat_led(beat_led)
            // , i2s(I2S_PIN_MUTE)
            , urlStream(wifi_ssid, wifi_password)
            , source(urlStream, urls, "audio/mp3")
            , multi_output(i2s)
            , decoder()
            , player(source, multi_output, decoder) 
        {};

        /*
        Web Radio Audio MUX controls
        */

        void mux_start() override;
        void mux_stop() override;
        void mux_copy() override;

        void volume(float level) override;

        virtual bool audio_active() override;
        virtual void audio_play() override;
        virtual void audio_pause() override;

        virtual void audio_next() override;
        virtual void audio_previous() override;

        void change_station(std::string station_name);

    private:
        bool beat_led;
        // I2SStream i2s;

        ICYStream urlStream;
        AudioSourceURL source;
        MultiOutput multi_output;
        MP3DecoderHelix decoder;
        AudioPlayer player;

};

#endif  // WEB_RADIO_H
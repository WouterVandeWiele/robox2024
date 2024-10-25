#ifndef WEB_RADIO_H
#define WEB_RADIO_H

#include "AudioTools.h"
#include "robox_mux_interface.h"
#include "AudioCodecs/CodecMP3Helix.h"
// #include "audio_mux.h"
// #include "wifi_credentials.h"
#include "general_definitions.h"

#include "robox_i2s.h"


// extern std::map<std::string, std::string> station_list;

// static const char *urls[] = {
//     "http://icecast.vrtcdn.be/mnm_hits-high.mp3"
// };

/*
Web radio Audio MUX controls
*/

class RoboxWebRadio: public MuxInterface {
    public:
        RoboxWebRadio(bool beat_led, float &volume_level, const std::vector<String> &urls, uint8_t pos)
            : MuxInterface(volume_level)
            , beat_led(beat_led)
            , pos(pos)
            , urls(urls)
            // , i2s(I2S_PIN_MUTE)
            // , urlStream(wifi_ssid, wifi_password)
            , urlStream()
            , source(urlStream, "audio/mp3")
            , multi_output(i2s)
            , decoder()
            , player(source, multi_output, decoder) 
        
        {
            source.addURL(urls.at(pos).c_str());
            // for (auto url : urls)
            // {
            //     source.addURL(url.c_str());
            // }
            // source.setIndex(pos);
        };

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

        virtual std::vector<String> song_list() override;

    private:
        bool beat_led;
        // I2SStream i2s;
        uint8_t pos;
        const std::vector<String> &urls;

        ICYStream urlStream;
        AudioSourceDynamicURL source;
        MultiOutput multi_output;
        MP3DecoderHelix decoder;
        AudioPlayer player;

};

#endif  // WEB_RADIO_H
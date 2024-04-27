#ifndef WEB_RADIO_H
#define WEB_RADIO_H

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
// #include "audio_mux.h"
#include "wifi_credentials.h"
#include "robox_mux_interface.h"
#include "general_definitions.h"


// extern std::map<std::string, std::string> station_list;


/*
Web radio Audio MUX controls
*/

class RoboxWebRadio: public MuxInterface {
    public:
        RoboxWebRadio(): i2s(I2S_PIN_MUTE) {};

        /*
        Web Radio Audio MUX controls
        */

        void mux_start();
        void mux_stop();
        void mux_copy();

        void change_station(std::string station_name);

    private:
        I2SStream i2s;

};

#endif  // WEB_RADIO_H
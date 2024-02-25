// #ifndef WEB_RADIO_H
// #define WEB_RADIO_H

// #include <map>

// #include "AudioTools.h"
// #include "AudioCodecs/CodecMP3Helix.h"
// #include "audio_mux.h"
// #include "wifi_credentials.h"
// #include "mux_interface.h"

// extern std::map<std::string, std::string> station_list;


// /*
// Web radio Audio MUX controls
// */

// class RoboxWebRadio: public MuxInterface {
//     public:
//         // RoboxWebRadio(I2SStream* i2s, I2SConfig* i2s_cfg) {
//         //     this->i2s = *i2s;
//         //     this->i2s_cfg = *i2s_cfg;
//         //     this->currentUrl(station_list["MNM Hits"]),
//         //     this->url(wifi_ssid, wifi_password),
//         //     this->mp3(),
//         //     this->dec(i2s, mp3),
//         //     this->copier(dec, url)
//         // };

//         RoboxWebRadio(I2SStream *i2s, I2SConfig i2s_cfg):
//             i2s_cfg(i2s_cfg),
//             currentUrl(station_list["MNM Hits"]),
//             url(wifi_ssid, wifi_password),
//             mp3(),
//             dec(*i2s, mp3),
//             copier(dec, url)
//         {
//         };
//         /*
//         Web Radio Audio MUX controls
//         */

//         void mux_stop();
//         void mux_start();
//         void mux_copy();

//         void setup();

//         void change_station(std::string station_name);

//     private:
//         // I2SStream i2s;
//         I2SConfig i2s_cfg;
//         std::string currentUrl;
//         URLStream url;
//         MP3DecoderHelix mp3;
//         EncodedAudioStream dec;
//         StreamCopy copier;


// };

// #endif  // WEB_RADIO_H
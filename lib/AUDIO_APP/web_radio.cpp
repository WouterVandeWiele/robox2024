// #include "AudioTools.h"
// #include "AudioCodecs/CodecMP3Helix.h"
// #include "web_radio.h"
// #include "wifi_credentials.h"
// #include "general_definitions.h"

// std::map<std::string, std::string> station_list = {
//     {"Radio 1", "http://icecast.vrtcdn.be/radio1-high.mp3"},
//     {"SRadio 1", "http://icecast.vrtcdn.be/radio1-mid.mp3"},
//     {"Radio 2", "http://icecast.vrtcdn.be/ra2ant-high.mp3"},
//     {"SRadio 2", "http://icecast.vrtcdn.be/ra2ant-mid.mp3"},
//     {"Studio Brussel", "http://icecast.vrtcdn.be/stubru-high.mp3"},
//     {"SStudio Brussel", "http://icecast.vrtcdn.be/stubru-mid.mp3"},
//     {"MNM", "http://icecast.vrtcdn.be/mnm-high.mp3"},
//     {"SMNM", "http://icecast.vrtcdn.be/mnm-mid.mp3"},
//     {"SMNM Hits", "http://icecast.vrtcdn.be/mnm_hits-mid.mp3"},
//     {"MNM Hits", "http://icecast.vrtcdn.be/mnm_hits-high.mp3"},
// };

// // RoboxWebRadio::RoboxWebRadio(I2SConfig i2s_config_source) {
//     // i2s_cfg = i2s_config_source;
//     // currentUrl = station_list["MNM Hits"];
//     // url = URLStream(wifi_ssid, wifi_password);
//     // dec = EncodedAudioStream(i2s_cfg, new MP3DecoderHelix()); // Decoding stream
//     // copier = StreamCopy(dec, url); // copy url to decoder
// // }

// void RoboxWebRadio::setup() {
//     ESP_LOGI(LOG_SD_TAG, ">>> Web Radio starting...");
//     dec.setNotifyAudioChange(i2s_cfg);

//     ESP_LOGI(LOG_BLE_TAG, "<<< Web Radio completed");
// }

// void RoboxWebRadio::mux_start() {
//     dec.begin();
//     url.begin(currentUrl.c_str());
// }

// void RoboxWebRadio::mux_stop() {
//     dec.end();
// }

// void RoboxWebRadio::mux_copy() {
//     copier.copy();
// }

// void RoboxWebRadio::change_station(std::string station_name) {
//     mux_stop();

//     try {
//         url.begin(station_list[station_name].c_str());
//     } catch (int e) {
//         ESP_LOGE(LOG_WEB_TAG, "(%d) Unable to lookup station URL %s", e, station_name);
//     }

//     mux_start();
// }
#include <map>

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "robox_web.h"
#include "robox_i2s.h"
#include "general_definitions.h"
#include "wifi_credentials.h"

// https://www.vrt.be/nl/aanbod/kijk-en-luister/radio-luisteren/streamingslinks-radio/
// std::map<std::string, std::string> station_list = {
//     {"Radio 1", "http://icecast.vrtcdn.be/radio1-high.mp3"},
//     {"SRadio 1", "http://icecast.vrtcdn.be/radio1-mid.mp3"},
//     {"Radio 2", "http://icecast.vrtcdn.be/ra2ant-high.mp3"},
//     {"SRadio 2", "http://icecast.vrtcdn.be/ra2ant-mid.mp3"},
//     {"Studio Brussel", "http://icecast.vrtcdn.be/stubru-high.mp3"},
//     {"SStudio Brussel", "http://icecast.vrtcdn.be/stubru-mid.mp3"},
//     {"MNM", "http://icecast.vrtcdn.be/mnm-high.mp3"},
//     {"SMNM", "http://icecast.vrtcdn.be/mnm-mid.mp3"},
//     {"MNM Hits", "http://icecast.vrtcdn.be/mnm_hits-high.mp3"},
//     {"SMNM Hits", "http://icecast.vrtcdn.be/mnm_hits-mid.mp3"},
// };


const char *urls[] = {
    "http://icecast.vrtcdn.be/mnm_hits-high.mp3"
};


ICYStream urlStream(wifi_ssid, wifi_password);
AudioSourceURL source(urlStream, urls, "audio/mp3");
MP3DecoderHelix decoder;
AudioPlayer player(source, i2s, decoder);

// Print Audio Metadata
void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}

void RoboxWebRadio::mux_start() {
    ESP_LOGI(LOG_SD_TAG, ">>> Web Radio starting...");
    i2s_setup();

    // setup player
    player.setMetadataCallback(printMetaData);
    player.begin();

    player.setVolume(0.2);

    ESP_LOGI(LOG_BLE_TAG, "<<< Web Radio completed");
}

void RoboxWebRadio::mux_stop() {
    player.end();
}

void RoboxWebRadio::mux_copy() {
    player.copy();
}

void RoboxWebRadio::change_station(std::string station_name) {
//     mux_stop();

//     try {
//         url.begin(station_list[station_name].c_str());
//     } catch (int e) {
//         ESP_LOGE(LOG_WEB_TAG, "(%d) Unable to lookup station URL %s", e, station_name);
//     }

//     mux_start();
}
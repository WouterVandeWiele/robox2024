#include <map>

#include <Arduino.h>
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "robox_audio_mux.h"
#include "robox_web.h"
#include "robox_i2s.h"
#include "general_definitions.h"
#include "wifi_credentials.h"
#include "robox_fft_beat.h"

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


extern RoboxAudioMux mux;





// ICYStream urlStream(wifi_ssid, wifi_password);
// AudioSourceURL web_source(urlStream, urls, "audio/mp3");
// MultiOutput web_multi_output(i2s);
// MP3DecoderHelix web_decoder;
// AudioPlayer web_player(web_source, web_multi_output, web_decoder);


// AudioPlayer web_player(web_source, i2s, web_decoder);
// extern AudioRealFFT fft;

// Print Audio Metadata
void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);

  if (toStr(type) == "Title") {
    // const std::lock_guard<std::mutex> lock(mux.meta_mutex);
    mux.meta.title = String((char*) str);
  }
}

void RoboxWebRadio::mux_start() {
    ESP_LOGI(LOG_SD_TAG, ">>> Web Radio starting...");
    i2s_setup();

    if (beat_led) {
      fft_beat_setup();
    }

    // Setup Multioutput
    // web_multi_output.add(i2s);
    if (beat_led) {
      multi_output.add(fft);
    }

    // i2s_driver_install((i2s_port_t)0);
    player.setMetadataCallback(printMetaData);
    player.setAutoFade(true);
    player.begin();

    player.setVolume(0.2);

    ESP_LOGI(LOG_BLE_TAG, "<<< Web Radio completed");
}

void RoboxWebRadio::mux_stop() {
    // player.setVolume(0);
    player.stop();
    // player.writeSilence(10000);
    delay(1000);
    player.end();
    WiFi.disconnect(true, false);
    WiFi.mode(WIFI_OFF);

    // i2s.end();
    // urlStream.end();
    // web_multi_output.end();
    // web_decoder.end();
    i2s_driver_uninstall((i2s_port_t)0);
}

void RoboxWebRadio::mux_copy() {
    player.copy();
}

void RoboxWebRadio::volume(float level) {
    player.setVolume(level);
}

bool RoboxWebRadio::audio_active() {
  return player.isActive();
}

void RoboxWebRadio::audio_play() {
    player.play();
}

void RoboxWebRadio::audio_pause() {
    player.stop();
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
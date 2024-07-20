#include <Arduino.h>

#include "robox_i2s.h"
#include "general_definitions.h"

#include "robox_audio_mux.h"
#include "robox_sd.h"
#include "general_definitions.h"

#include "robox_fft_beat.h"
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"



// #define USE_HELIX 
// #define USE_SDFAT

const char *startFilePath="/";
const char* ext="mp3";

// AudioSourceSDFAT sd_source(startFilePath, ext);


// MultiOutput sd_multi_output(i2s);
// MP3DecoderHelix sd_decoder;
// AudioSourceSD sd_source(startFilePath, ext);
// AudioPlayer sd_player(sd_source, sd_multi_output, sd_decoder);


// AudioPlayer sd_player(sd_source, i2s, sd_decoder);
extern AudioRealFFT fft;

extern RoboxAudioMux mux;



static void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);

  if (toStr(type) == "Title") {
    // const std::lock_guard<std::mutex> lock(mux.meta_mutex);
    mux.meta.title = String((char*) str);
  }
}


void RoboxSD::mux_start() {
    ESP_LOGI(LOG_SD_TAG, ">>> SD starting...");

    i2s_setup();

    if (beat_led) {
      fft_beat_setup();
    }

    // Setup Multioutput
    // sd_multi_output.add(i2s);
    if (beat_led) {
      multi_output.add(fft);
    }

    // setup player
    player.setMetadataCallback(printMetaData);
    player.setAutoFade(true);
    player.begin();

    player.setVolume(0.2);

    ESP_LOGI(LOG_BLE_TAG, "<<< SD setup completed");
}

void RoboxSD::mux_stop() {
  player.setVolume(0);
  player.stop();
  player.end();

  i2s_driver_uninstall((i2s_port_t)0);
}

void RoboxSD::mux_copy() {
  player.copy();
}

void RoboxSD::volume(float level) {
  player.setVolume(level);
}

bool RoboxSD::audio_active() {
  return player.isActive();
}

void RoboxSD::audio_play() {
    player.play();
}

void RoboxSD::audio_pause() {
    player.stop();
}
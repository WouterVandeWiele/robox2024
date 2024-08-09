#include <Arduino.h>

#include "robox_i2s.h"
#include "general_definitions.h"

#include "robox_audio_mux.h"
#include "robox_sd.h"
#include "general_definitions.h"

#include "robox_fft_beat.h"
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"

#include "lcd_screen.h"

// #define USE_HELIX 
// #define USE_SDFAT

const char *startFilePath="/";
const char* ext="mp3";

extern AudioRealFFT fft;

extern RoboxAudioMux mux;


// Print Audio Metadata (same as in robox_web.cpp)
static void printMetaData(MetaDataType type, const char* str, int len){
  std::lock_guard<std::mutex> lck(meta_data_mtx);
  
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);

  switch (type)
  {
  case Title:
    mux.meta.title = String((char*) str);
    lcd_invalidate(INVALIDATE_ALL);
    break;

  case Artist:
    break;

  case Album:
    break;

  case Genre:
    break;

  case Name:
    break;

  case Description:
    break;
  
  default:
    break;
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
  // player.setVolume(0);
  player.stop();
  delay(1000);
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
    is_audio_paused = false;
    digitalWrite(I2S_PIN_MUTE, HIGH);
    delay(300);
    player.play();
}

void RoboxSD::audio_pause() {
    is_audio_paused = true;
    digitalWrite(I2S_PIN_MUTE, LOW);
    delay(300);
    player.stop();
}

void RoboxSD::audio_next() {
  audio_pause();
  player.next();
  audio_play();
}

void RoboxSD::audio_previous() {
  audio_pause();
  player.previous();
  audio_play();
}

bool RoboxSD::is_sd_inserted() {
  return digitalRead(USD_CARD_DETECT) ? true : false;
}
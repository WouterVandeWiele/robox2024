/**
 * @file player-url-i2s.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-player/player-url-i2s/README.md
 * 
 * @author Phil Schatzmann
 * @copyright GPLv3
 */


#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "robox_i2s.h"
#include "fft_beat.h"
#include "general_definitions.h"
#include "wifi_credentials.h"

// const char *urls[] = {
//   "http://stream.srg-ssr.ch/m/rsj/mp3_128",
//   "http://stream.srg-ssr.ch/m/drs3/mp3_128",
//   "http://stream.srg-ssr.ch/m/rr/mp3_128",
//   "http://sunshineradio.ice.infomaniak.ch/sunshineradio-128.mp3",
//   "http://streaming.swisstxt.ch/m/drsvirus/mp3_128"
// };

const char *urls[] = {
    "http://icecast.vrtcdn.be/mnm_hits-high.mp3"
};

ICYStream urlStream(wifi_ssid, wifi_password);
AudioSourceURL source(urlStream, urls, "audio/mp3");
MultiOutput multi_output;
MP3DecoderHelix decoder;
AudioPlayer player(source, multi_output, decoder);
extern AudioRealFFT fft; // or AudioKissFFT or others


// Print Audio Metadata
void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}


// Arduino setup
void player_setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

    i2s_setup();
    fft_beat_setup();

  pinMode(I2S_PIN_MUTE, OUTPUT);
  digitalWrite(I2S_PIN_MUTE, HIGH);

  // Setup Multioutput
  multi_output.add(fft);
  multi_output.add(i2s);

  // setup player
  player.setMetadataCallback(printMetaData);
  player.begin();

  player.setVolume(0.2);
  // player.setVolume(0.6);
  // player.setVolume(1.0);
}

// Arduino loop
void player_loop() {
  player.copy();
}
/**
 * @file player-sdfat-i2s.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-player/player-sdfat-i2s/README.md
 * 
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

#define USE_HELIX 
#define USE_SDFAT


#include "AudioTools.h"
#include "AudioLibs/AudioSourceSDFAT.h"
#include "AudioCodecs/CodecMP3Helix.h"



#include "robox_i2s.h"
#include "general_definitions.h"


const char *startFilePath="/";
const char* ext="mp3";
AudioSourceSDFAT source(startFilePath, ext);
MP3DecoderHelix decoder;
AudioPlayer player(source, i2s, decoder);


static void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}

void player_setup() {
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  i2s_setup();

  // setup player
  player.setMetadataCallback(printMetaData);
  player.begin();

  player.setVolume(0.2);
}

void player_loop() {
  player.copy();
}
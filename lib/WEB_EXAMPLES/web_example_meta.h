/**
 * @file streams-url_mp3-out.ino
 * @author Phil Schatzmann
 * @brief read MP3 stream from url and output of metadata only: There is no audio output!
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

// install https://github.com/pschatzmann/arduino-libhelix.git

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "i2s.h"
#include "wifi_credentials.h"
#include "general_definitions.h"



ICYStream url(wifi_ssid, wifi_password);
MetaDataOutput out; // final output of decoded stream
StreamCopy copier(out, url); // copy url to decoder

// I2SStream i2s; // final output of decoded stream
// VolumeStream volume(i2s);
// EncodedAudioStream dec(&i2s, new MP3DecoderHelix()); // Decoding stream
// StreamCopy copier(dec, url); // copy url to decoder

// callback for meta data
void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}

void web_setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

// mp3 radio
  url.httpRequest().header().put("Icy-MetaData","1");
  url.begin("http://icecast.vrtcdn.be/mnm_hits-high.mp3","audio/mp3");

  out.setCallback(printMetaData);
  out.begin(url.httpRequest());
}

void web_loop(){
  copier.copy();
}
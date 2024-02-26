/**
 * @file streams-url_mp3-metadata2.ino
 * @author Phil Schatzmann
 * @brief read MP3 stream from url and output metadata and audio! 
 * The used mp3 file contains ID3 Metadata!
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

// install https://github.com/pschatzmann/arduino-libhelix.git

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "i2s.h"
#include "general_definitions.h"
#include "wifi_credentials.h"

//                            -> EncodedAudioStream -> I2SStream
// URLStream -> MultiOutput -|
//                            -> MetaDataOutput

URLStream url(wifi_ssid, wifi_password);
MetaDataOutput out1; // final output of metadata
I2SStream i2s; // I2S output
// VolumeStream volume(i2s);
EncodedAudioStream out2dec(&i2s, new MP3DecoderHelix()); // Decoding stream
MultiOutput out;
StreamCopy copier(out, url); // copy url to decoder

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

  // setup multi output
  out.add(out1);
  out.add(out2dec);

  // setup input
//   url.begin("https://pschatzmann.github.io/Resources/audio/audio.mp3","audio/mp3");
  url.begin("http://icecast.vrtcdn.be/mnm_hits-high.mp3","audio/mp3");
  // setup metadata
  out1.setCallback(printMetaData);
  out1.begin(url.httpRequest());

  // setup i2s
    auto i2s_cfg = i2s.defaultConfig(TX_MODE);
    i2s_cfg.sample_rate = my_i2s_config.sample_rate;
    i2s_cfg.bits_per_sample = my_i2s_config.bits_per_sample;
    i2s_cfg.channel_format = my_i2s_config.channel_format;
    // i2s_cfg.channel_format = i2s_config.communication_format;

    i2s_cfg.buffer_count = my_i2s_config.dma_buf_count;
    i2s_cfg.buffer_size = my_i2s_config.dma_buf_len;
    i2s_cfg.use_apll = my_i2s_config.use_apll;
    i2s_cfg.auto_clear = my_i2s_config.tx_desc_auto_clear;
    i2s_cfg.pin_bck = my_i2s_pin_config.bck_io_num;
    i2s_cfg.pin_ws = my_i2s_pin_config.ws_io_num;
    i2s_cfg.pin_data = my_i2s_pin_config.data_out_num;

    i2s.begin(i2s_cfg);

  // setup I2S based on sampling rate provided by decoder
  out2dec.setNotifyAudioChange(i2s);
  out2dec.begin();

  // // set initial volume
  // volume.begin(i2s_cfg); // we need to provide the bits_per_sample and channels
  // volume.setVolume(0.1);

}

void web_loop(){
  copier.copy();
}
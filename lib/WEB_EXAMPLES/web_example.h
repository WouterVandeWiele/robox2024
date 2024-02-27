/**
 * @file streams-url_mp3-i2s.ino
 * @author Phil Schatzmann
 * @brief decode MP3 stream from url and output it on I2S
 * @version 0.1
 * @date 2021-96-25
 * 
 * @copyright Copyright (c) 2021
 */

// install https://github.com/pschatzmann/arduino-libhelix.git

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "i2s.h"
#include "wifi_credentials.h"
#include "general_definitions.h"


URLStream url(wifi_ssid, wifi_password);
// I2SStream i2s; // final output of decoded stream
VolumeStream volume(i2s);
EncodedAudioStream dec(&i2s, new MP3DecoderHelix()); // Decoding stream
StreamCopy copier(dec, url); // copy url to decoder


void web_setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  // // setup i2s
  //   auto i2s_cfg = i2s.defaultConfig(TX_MODE);
  //   i2s_cfg.sample_rate = my_i2s_config.sample_rate;
  //   i2s_cfg.bits_per_sample = my_i2s_config.bits_per_sample;
  //   i2s_cfg.channel_format = my_i2s_config.channel_format;
  //   // i2s_cfg.channel_format = i2s_config.communication_format;

  //   i2s_cfg.buffer_count = my_i2s_config.dma_buf_count;
  //   i2s_cfg.buffer_size = my_i2s_config.dma_buf_len;
  //   i2s_cfg.use_apll = my_i2s_config.use_apll;
  //   i2s_cfg.auto_clear = my_i2s_config.tx_desc_auto_clear;
  //   i2s_cfg.pin_bck = my_i2s_pin_config.bck_io_num;
  //   i2s_cfg.pin_ws = my_i2s_pin_config.ws_io_num;
  //   i2s_cfg.pin_data = my_i2s_pin_config.data_out_num;

  //   i2s.begin(i2s_cfg);

  i2s_setup();

  // setup I2S based on sampling rate provided by decoder
  dec.setNotifyAudioChange(i2s);
  dec.begin();

    volume.setVolume(0.01);
// mp3 radio
  url.begin("http://icecast.vrtcdn.be/mnm_hits-high.mp3","audio/mp3");

}

void web_loop(){
  copier.copy();
}
#ifndef I2S_H
#define I2S_H

#include "AudioTools.h"
#include "general_definitions.h"


static i2s_config_t my_i2s_config = {
    .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100, // updated automatically by A2DP
    .bits_per_sample = (i2s_bits_per_sample_t)16,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0 | ESP_INTR_FLAG_LEVEL3,
    .dma_buf_count = 16, // 8,
    .dma_buf_len = 128, //64,
    .use_apll = true,
    .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
};

i2s_pin_config_t my_i2s_pin_config = {
    .bck_io_num = I2S_PIN_BCLK,
    .ws_io_num = I2S_PIN_WS,
    .data_out_num = I2S_PIN_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE
};

// static i2s_config_t my_i2s_config;

// i2s_pin_config_t my_i2s_pin_config;

// void i2s_setup();
//  audio_tools::AudioInfoSupport

// i2s.defaultConfig(TX_MODE);
// void i2s_setup() {
//     auto i2s_cfg = i2s.defaultConfig(TX_MODE);
//     i2s_cfg.sample_rate = i2s_config.sample_rate;
//     i2s_cfg.bits_per_sample = i2s_config.bits_per_sample;
//     i2s_cfg.channel_format = i2s_config.channel_format;
//     // i2s_cfg.channel_format = i2s_config.communication_format;
    
//     i2s_cfg.buffer_count = i2s_config.dma_buf_count;
//     i2s_cfg.buffer_size = i2s_config.dma_buf_len;
//     i2s_cfg.use_apll = i2s_config.use_apll;
//     i2s_cfg.auto_clear = i2s_config.tx_desc_auto_clear;
//     i2s_cfg.pin_bck = i2s_pin_config.bck_io_num;
//     i2s_cfg.pin_ws = i2s_pin_config.ws_io_num;
//     i2s_cfg.pin_data = i2s_pin_config.data_out_num;
    
//     i2s.begin(i2s_cfg);
// }


#endif  // I2S_H
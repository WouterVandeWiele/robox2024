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
    .intr_alloc_flags = 0, // default interrupt priority
    .dma_buf_count = 8,
    .dma_buf_len = 64,
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

// static i2s_config_t my_i2s_config = {
//     .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
//     .sample_rate = 44100, // updated automatically by A2DP
//     .bits_per_sample = (i2s_bits_per_sample_t)16,
//     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
//     .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
//     .intr_alloc_flags = 0, // default interrupt priority
//     .dma_buf_count = 8,
//     .dma_buf_len = 64,
//     .use_apll = true,
//     .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
// };

// i2s_pin_config_t my_i2s_pin_config = {
//     .bck_io_num = I2S_PIN_BCLK,
//     .ws_io_num = I2S_PIN_WS,
//     .data_out_num = I2S_PIN_DATA,
//     .data_in_num = I2S_PIN_NO_CHANGE
// };


#endif  // I2S_H
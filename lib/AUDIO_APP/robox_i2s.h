#ifndef I2S_H
#define I2S_H

#include "AudioTools.h"
#include "general_definitions.h"

#define ROBOX_I2S_MODE (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX)
#define ROBOX_I2S_SAMPLE_RATE 44100
#define ROBOX_I2S_BITS_PER_SAMPLE (i2s_bits_per_sample_t)16
#define ROBOX_I2S_CHANNEL_FORMAT I2S_CHANNEL_FMT_RIGHT_LEFT
#define ROBOX_I2S_COMMUNICATION_FORMAT (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S)
#define ROBOX_I2S_INTERRUPT_FLAGS 0 | ESP_INTR_FLAG_LEVEL3
#define ROBOX_I2S_DMA_BUFFER_COUNT 16
#define ROBOX_I2S_DMA_BUFFER_LEN 128
#define ROBOX_I2S_USE_AUDIO_PLL true
#define ROBOX_I2S_TX_DESCRIPTOR_AUTO_CLEAR true


extern I2SStream i2s;

extern i2s_config_t my_i2s_config;

extern i2s_pin_config_t my_i2s_pin_config;


void i2s_setup();


#endif  // I2S_H
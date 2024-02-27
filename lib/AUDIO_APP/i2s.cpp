#include "AudioTools.h"
#include "i2s.h"
#include "general_definitions.h"

I2SStream i2s(I2S_PIN_MUTE);

static VolumeStream volume(i2s);

i2s_config_t my_i2s_config = {
    .mode = ROBOX_I2S_MODE,
    .sample_rate = ROBOX_I2S_SAMPLE_RATE, // updated automatically by A2DP
    .bits_per_sample = ROBOX_I2S_BITS_PER_SAMPLE,
    .channel_format = ROBOX_I2S_CHANNEL_FORMAT,
    .communication_format = ROBOX_I2S_COMMUNICATION_FORMAT,
    .intr_alloc_flags = ROBOX_I2S_INTERRUPT_FLAGS,
    .dma_buf_count = ROBOX_I2S_DMA_BUFFER_COUNT,
    .dma_buf_len = ROBOX_I2S_DMA_BUFFER_LEN,
    .use_apll = ROBOX_I2S_USE_AUDIO_PLL,
    .tx_desc_auto_clear = ROBOX_I2S_TX_DESCRIPTOR_AUTO_CLEAR // avoiding noise in case of data unavailability
};

i2s_pin_config_t my_i2s_pin_config = {
    .bck_io_num = I2S_PIN_BCLK,
    .ws_io_num = I2S_PIN_WS,
    .data_out_num = I2S_PIN_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE
};

void i2s_setup() {

    I2SConfig i2s_cfg = i2s.defaultConfig(TX_MODE);
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

    // auto volume_cfg = volume.begin(i2s_cfg); 
    // volume_cfg.allow_boost = false;// we need to provide the bits_per_sample and channels
    // volume.setVolume(0.01);
}
#include "AudioTools.h"
#include "general_definitions.h"

// #include "AudioTools.h"
// #include "i2s.h"

// void i2s_setup() {

// }

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
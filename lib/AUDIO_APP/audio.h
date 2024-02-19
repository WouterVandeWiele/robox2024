/**
 * @file basic-a2dp-audioi2s.ino
 * @brief A2DP Sink with output to I2SStream. This example is of small value
 * since my Bluetooth Library already provides I2S output out of the box.
 *
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

// robox audio architecture overview [docs/audio_overview.excalidraw.png]

#include "BluetoothA2DPSink.h"
#include "AudioTools.h"

#define USE_AUDIO_LOGGING false

// pin configuration
#define PIN_MUTE 22
#define PIN_DATA 32
#define PIN_BCLK 33
#define PIN_WS 25
#define A2DP_BLE_NAME "a2dp-i2s"

BluetoothA2DPSink a2dp_sink;
I2SStream i2s(PIN_MUTE);

// Write data to I2S
// void read_data_stream(const uint8_t *data, uint32_t length) {
//   i2s.write(data, length);
// }

// callback used by A2DP to provide the sound data
void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
  Serial.printf("> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);
}


void audio_setup() {
  Serial.begin(115200);
  Serial.println(">>> I2S started...");
  // AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  // register callback
  // a2dp_sink.set_stream_reader(read_data_stream, false);

  // register data callback
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);

  static i2s_config_t i2s_config = {
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

  i2s_pin_config_t my_pin_config = {
    .bck_io_num = PIN_BCLK,
    .ws_io_num = PIN_WS,
    .data_out_num = PIN_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  a2dp_sink.set_pin_config(my_pin_config);

  // Start Bluetooth Audio Receiver
  a2dp_sink.set_i2s_config(i2s_config);
  a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM | ESP_AVRC_MD_ATTR_TRACK_NUM | ESP_AVRC_MD_ATTR_NUM_TRACKS | ESP_AVRC_MD_ATTR_GENRE | ESP_AVRC_MD_ATTR_PLAYING_TIME);
  a2dp_sink.set_auto_reconnect(true);
  a2dp_sink.start(A2DP_BLE_NAME);

}
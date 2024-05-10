/**
 * @file basic-a2dp-fft.ino
 * @brief A2DP Sink with output to FFT.   
 * For details see the FFT Wiki: https://github.com/pschatzmann/arduino-audio-tools/wiki/FFT
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

// https://github.com/leandcesar/PeakDetection

#include "AudioTools.h"
#include "AudioLibs/AudioRealFFT.h" // or any other supported inplementation
#include "BluetoothA2DPSink.h"
#include "robox_i2s.h"
#include <PeakDetection.h>
#include <FastLED.h>

#include "fft_beat.h"

BluetoothA2DPSink a2dp_sink;
extern AudioRealFFT fft; // or AudioKissFFT or others


// Provide data to FFT
void writeDataStream(const uint8_t *data, uint32_t length) {
  i2s.write(data, length);
  fft.write(data, length);
}


void player_setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);



  // register callback
  a2dp_sink.set_stream_reader(writeDataStream, false);

  // Start Bluetooth Audio Receiver
  Serial.print("starting a2dp-fft...");
  a2dp_sink.set_auto_reconnect(false);
  a2dp_sink.start("a2dp-fft");

  i2s_setup();
  fft_beat_setup(a2dp_sink.sample_rate());
}

void player_loop() {
  if (Serial.available() > 0) {
    // reconfigure sine frequency
    char buffer[20];
    char buffer2[20];
    int len = Serial.readBytesUntil('\n', (char*)buffer, 20);
    Serial.printf("reconfigure: %s Hz\n", buffer);

    memset(buffer2, '\0', 20);
    memcpy(buffer2, &buffer[1], 19);
    memset(buffer, '\0', 20);

    if (buffer[0] == 'l') lag = atoi(buffer2);
    if (buffer[0] == 't') threshold = atoi(buffer2);
    if (buffer[0] == 'i') influence = atof(buffer2);

    peakb0.begin(lag, threshold, influence);
  }
}
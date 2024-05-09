// /**
//  * @file streams-sd_mp3-i2s.ino
//  * @author Phil Schatzmann
//  * @brief decode MP3 file and output it on I2S
//  * @version 0.1
//  * @date 2021-96-25
//  * 
//  * @copyright Copyright (c) 2021 
//  */


// #include <SPI.h>
// #include <SD.h>
// #include "AudioTools.h"
// #include "AudioCodecs/CodecMP3Helix.h"

// #include "robox_i2s.h"
// #include "general_definitions.h"

// #include "AudioLibs/AudioRealFFT.h" // or AudioKissFFT

// // AudioBoardStream kit(AudioKitEs8388V1);  // Audio source
// AudioRealFFT fft; // or AudioKissFFT
// // StreamCopy copier(fft, kit);  // copy mic to tfl
// int channels = 2;
// int samples_per_second = 44100;
// int bits_per_sample = 16;
// float value=0;


// // I2SStream i2s; // final output of decoded stream
// // EncodedAudioStream decoder(&i2s, new MP3DecoderHelix()); // Decoding stream
// StreamCopy copier; 
// File audioFile;

// void player_setup(){
//   Serial.begin(115200);
//   AudioLogger::instance().begin(Serial, AudioLogger::Info);  

//   // setup file
//   SD.begin();
//   audioFile = SD.open("/");

//   // setup i2s
// //   auto config = i2s.defaultConfig(TX_MODE);
// //   i2s.begin(config);
//   // i2s_setup();

//   // setup I2S based on sampling rate provided by decoder
//   // decoder.begin();

//   // begin copy
//   copier.begin(fft, audioFile);

// }

// void player_loop(){
//   if (!copier.copy()) {
//     stop();
//   }
// }

/**
 * @file streams-generator-volume.ino
 * @author Phil Schatzmann
 * @brief Determines the output volume (=amplitude)
 * @copyright GPLv3
 */
 
#include "AudioTools.h"
#include "AudioLibs/AudioRealFFT.h" // or AudioKissFFT
// #include "AudioLibs/LEDOutput.h"

AudioRealFFT fft; // or AudioKissFFT
// AudioFFTBase fft; // or AudioKissFFT
// StreamCopy copier(fft, kit);  // copy mic to tfl
int channels = 2;
int samples_per_second = 44100;
int bits_per_sample = 16;
float value=0;
float sine_amplitude = 200;
float frequency = 4e3;

Vector<float> magnitudes{0};

AudioInfo info(samples_per_second, channels, bits_per_sample);
SineWaveGenerator<int16_t> sineWave(sine_amplitude);                // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
// VolumeOutput out; 
StreamCopy copier(fft, sound, 1024*2);                     // copies sound into VolumeOutput


// display fft result
void fftResult(AudioFFTBase &fft) {
    int size = fft.size() / 2;

    auto result = fft.result();
    if (result.magnitude>100) {
        Serial.printf("\n>freq:%.2f\n>mag:%f\n", result.frequency, result.magnitude);

    float *magnitudes = fft.magnitudes();

    for (int b=0; b<size; b++) {
      Serial.printf(">bin%d_amp:%.2f\n", b, magnitudes[b]);
    }
        // Serial.print("Freq: ");
        // Serial.print(result.frequency);
        // Serial.print(" ");
        // Serial.print(result.magnitude);  
        // Serial.print(" => ");
        // Serial.print(result.frequencyAsNote(diff));
        // Serial.print( " diff: ");
        // Serial.println(diff);
        // Serial.println("")
    }
}

// Arduino Setup
void player_setup(void) {  
  // Open Serial 
  Serial.begin(115200);
  while(!Serial);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  // start Volume Output
  // out.begin(info);

    // Setup FFT
  auto tcfg = fft.defaultConfig();
  // tcfg.length = 8192;
  tcfg.length = 128;
  tcfg.channels = info.channels;
  tcfg.sample_rate = info.sample_rate;
  tcfg.bits_per_sample = info.bits_per_sample;
  tcfg.callback = &fftResult;
  fft.begin(tcfg);

  // Setup sine wave
  sineWave.begin(info, frequency);
  Serial.println("started...");
}

// Arduino loop - copy sound to out 
void player_loop() {
  copier.copy();
  // Serial.print(out.volume()); // overall
  // Serial.print(" ");
  // Serial.print(out.volume(0)); // left
  // Serial.print(" ");
  // Serial.println(out.volume(1)); // right

  if (Serial.available() > 0) {
    // reconfigure sine frequency
    char buffer[20];
    int len = Serial.readBytesUntil('\n', (char*)buffer, 20);
    Serial.printf("reconfigure sine frequency: %s Hz\n", buffer);
    sineWave.end();
    sineWave.begin(info, atoi(buffer));
  }

}
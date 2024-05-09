/**
 * @file streams-generator-volume.ino
 * @author Phil Schatzmann
 * @brief Determines the output volume (=amplitude)
 * @copyright GPLv3
 */
 
#include "AudioTools.h"
#include "AudioLibs/AudioRealFFT.h" // or AudioKissFFT
#include <PeakDetection.h>

PeakDetection peakDetection;                     // create PeakDetection object
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

        // for (int b=0; b<size; b++) {
        //   Serial.printf(">bin%d_amp:%.2f\n", b, magnitudes[b]);
        // }

        peakDetection.add(magnitudes[0]);                     // adds a new data point
        int peak = peakDetection.getPeak();          // 0, 1 or -1
        double filtered = peakDetection.getFilt();   // moving average

        Serial.printf(">bin0:%.2f\n>peak:%d\n>filter:%.2f\n", magnitudes[0], peak, filtered);
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

  peakDetection.begin(48, 3, 0.6);               // sets the lag, threshold and influence
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
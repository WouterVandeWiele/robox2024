/**
 * @file basic-a2dp-fft.ino
 * @brief A2DP Sink with output to FFT.   
 * For details see the FFT Wiki: https://github.com/pschatzmann/arduino-audio-tools/wiki/FFT
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

// https://github.com/garrickhogrebe/ESP32FastLEDSketches/blob/main/Beat-Detection_test.ino#L38

#include "AudioTools.h"
#include "AudioLibs/AudioRealFFT.h" // or any other supported inplementation
#include "BluetoothA2DPSink.h"
#include "robox_i2s.h"
// #include <PeakDetection.h>

// #define FILTER_POSITIONS 5

// PeakDetection peakb0;
// PeakDetection peakb1;

int pos = 0;
float testF = 2.2;
bool newAudio;
float bandValues;
float prevBandValues;
int flux = 0;
int realFlux = 0;
int prevFlux = 0;
int slowDecayPeak = 0;
int fastDecayPeak = 0;
int slowDecayMaxFlux = 0;
int fastDecayMaxFlux = 0;
int averagePosFlux = 0;
int prevBandVal = 0;
int bandVal = 0;
int lastPart = 0;
bool beat = true;
bool fluxHasDecreased = true;

BluetoothA2DPSink a2dp_sink;
AudioRealFFT fft; // or any other supported inplementation

// peak detection parameters
int lag = 40;
int threshold = 3;
double influence = 0.5;


// Provide data to FFT
void writeDataStream(const uint8_t *data, uint32_t length) {
  i2s.write(data, length);
  fft.write(data, length);
}

// display fft result
void fftResult(AudioFFTBase &fft){

        float *magnitudes = fft.magnitudes();


        prevBandValues = bandValues;

    bandVal = magnitudes[0];
    
    realFlux = bandVal - prevBandVal;
    if(realFlux > 0){
      flux = realFlux;
      averagePosFlux = (averagePosFlux*5 + 5*flux)/10;
    }else {
      flux = 0;
      averagePosFlux = (95*averagePosFlux)/100;
    }

    if(flux > fastDecayMaxFlux){
      fastDecayMaxFlux = flux;
      if(fluxHasDecreased){
        beat = true;
      }
    }else fastDecayMaxFlux = (fastDecayMaxFlux*8)/10;

    if(flux > slowDecayMaxFlux){
      slowDecayMaxFlux = flux;
    }else {
      if(flux == 0){
        slowDecayMaxFlux = slowDecayMaxFlux*995/1000;
      }else{
        slowDecayMaxFlux = (slowDecayMaxFlux*999 + flux)/1000;
      }    
    }

    if(bandVal > fastDecayPeak){
      fastDecayPeak = bandVal;
    }else fastDecayPeak = (fastDecayPeak*9)/10;

    if(bandVal > slowDecayPeak){
      slowDecayPeak = bandVal;
    }else slowDecayPeak = (slowDecayPeak*999 + bandVal)/1000;

    if(fastDecayMaxFlux < slowDecayMaxFlux*3/8){
      beat = false;
    }

    if(fastDecayMaxFlux < averagePosFlux || flux == 0){
      beat = false;
    }
    
    if(flux < prevFlux){
      fluxHasDecreased = true;
    }
    prevFlux = flux;
    prevBandVal = bandVal;

    if(flux > 0){
      //beat = true;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Uncomment this section to see results in serial monitor~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Serial.printf(">flux:%d\n>fastDecayMaxFlux:%d\n>slowDecayMaxFlux:%d\n>averagePosFlux:%d\n>beat:%d\n", flux, fastDecayMaxFlux, slowDecayMaxFlux*3/8, averagePosFlux, beat);
    
    
    // Serial.print(flux);
    // Serial.print(",");
    // Serial.print(fastDecayMaxFlux);
    // Serial.print(",");
    // Serial.print(slowDecayMaxFlux*3/8);
    // Serial.print(",");
    // Serial.println(averagePosFlux);
}

void player_setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // Setup FFT
  auto tcfg = fft.defaultConfig();
  // tcfg.length = 4096;
  // tcfg.length = 256;
  tcfg.length = 128;
  // tcfg.length = 512;

  tcfg.channels = 2;
  tcfg.sample_rate = a2dp_sink.sample_rate();;
  tcfg.bits_per_sample = 16;
  tcfg.callback = &fftResult;
  fft.begin(tcfg);


  // register callback
  a2dp_sink.set_stream_reader(writeDataStream, false);

  // Start Bluetooth Audio Receiver
  Serial.print("starting a2dp-fft...");
  a2dp_sink.set_auto_reconnect(false);
  a2dp_sink.start("a2dp-fft");

  i2s_setup();

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

  }
}

#ifndef ROBOX_FFT_BEAT
#define ROBOX_FFT_BEAT

#include "AudioLibs/AudioRealFFT.h" // or any other supported inplementation
#include "general_definitions.h"
#include <PeakDetection.h>
#include <FastLED.h>


// #define BEAT_TELEMETRY

#define NUM_LEDS 6
CRGB leds[NUM_LEDS];

PeakDetection peakb0;

AudioRealFFT fft; // or any other supported inplementation

// peak detection parameters
int lag = 30;
int threshold = 3;
double influence = 0.6;


// peak detection variables
/* max_filter is used as a shift register
 * each fft caclulation we shift in the peak value (boolean)
 * to detect if we have trailing peaks we only change the LED colors if this variable reads 0x1
 */
#define FILTER_POSITIONS 0b00111111     // determines how many bits we use in the shift register
uint8_t max_filter = 0;


// https://github.com/Resseguie/FastLED-Patterns/blob/master/fastled-patterns.ino#L269

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

CRGB randomColor(){
  return Wheel(random(256)); 
}

void allRandom(){
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = randomColor();
  }
  FastLED.show(); 
}


// display fft result
void fftResult(AudioFFTBase &fft) {
    float *magnitudes = fft.magnitudes();

    peakb0.add(magnitudes[0]);

    max_filter = FILTER_POSITIONS & ((max_filter << 1) | peakb0.getPeak());

    if (max_filter == 0x1) {
      allRandom();
    }
    
    #if defined(BEAT_TELEMETRY)
    Serial.printf(">bin0:%.2f\n>maxf:%.2f\n>peak0:%.2f\n>filter0:%.2f\n", magnitudes[0], ((max_filter == 0x01) ? magnitudes[0] : 0) , (peakb0.getPeak()*magnitudes[0]), peakb0.getFilt(), peakb0.getEpsilon());
    #endif
}


void fft_beat_setup(int samplerate = 44100) {
    // Setup FFT
  auto tcfg = fft.defaultConfig();
  // tcfg.length = 4096;
  // tcfg.length = 256;
  tcfg.length = 128;
  // tcfg.length = 512;

  tcfg.channels = 2;
  tcfg.sample_rate = samplerate;
  tcfg.bits_per_sample = 16;
  tcfg.callback = &fftResult;
  fft.begin(tcfg);

    /*
  
  The algorithm takes 3 inputs:
    - lag: is the lag of the moving window. This parameter determines how much 
            your data will be smoothed and how adaptive the algorithm is to changes 
            in the long-term average of the data. The more stationary your data is, 
            the more lags you should include. If your data contains time-varying trends, 
            you should consider how quickly you want the algorithm to adapt to these trends.
    - threshold: this parameter is the number of standard deviations from the 
            moving mean above which the algorithm will classify a new datapoint 
            as being a signal. This parameter should be set based on how many 
            signals you expect. The threshold therefore directly influences how 
            sensitive the algorithm is and thereby also how often the algorithm signals.
    - influence: is the z-score at which the algorithm signals. 
            This parameter determines the influence of signals on the algorithm's 
            detection threshold. If put at 0, signals have no influence on the 
            threshold, such that future signals are detected based on a threshold 
            that is calculated with a mean and standard deviation that is not 
            influenced by past signals. You should put the influence parameter 
            somewhere between 0 and 1, depending on the extent to which signals 
            can systematically influence the time-varying trend of the data.
*/

  // sets the (lag, threshold, influence)
  peakb0.begin(lag, threshold, influence);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

}



#endif  // ROBOX_FFT_BEAT
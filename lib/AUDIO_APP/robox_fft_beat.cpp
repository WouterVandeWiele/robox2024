#include "robox_fft_beat.h"
#include "AudioTools.h"
#include "AudioLibs/AudioRealFFT.h"
#include "general_config.h"
#include "general_definitions.h"
#include <PeakDetection.h>
#define FASTLED_FORCE_SOFTWARE_SPI
#include <FastLED.h>

#include <random>

#include "robox_led_motor_controller.h"
extern LedMotorController led_motor_controller;

#include "robox_motor.h"
extern RoboxMotor* motor;

static CRGB leds[NUM_LEDS];

// peak detection parameters
static int lag = 30;
static int threshold = 3;
static double influence = 0.6;

// static uint32_t fft_counter = 0;
// static uint32_t fft_timekeeper = 0;

// static uint32_t beat_counter = 0;
uint32_t beat_loop_timestamp = 0;
static uint32_t beat_timekeeper = 0;


// peak detection variables
/* max_filter is used as a shift register
 * each fft caclulation we shift in the peak value (boolean)
 * to detect if we have trailing peaks we only change the LED colors if this variable reads 0x1
 */
static uint8_t max_filter = 0;


PeakDetection peakb0;
AudioRealFFT fft;


// https://github.com/Resseguie/FastLED-Patterns/blob/master/fastled-patterns.ino#L269

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
static CRGB Wheel(byte WheelPos) {
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

static CRGB randomColor() {
  return Wheel(random(256)); 
}

void allRandom() {
  for(int i=0; i<NUM_LEDS; i++){
    leds[i] = randomColor();
  }
  FastLED.show(); 
}


// // display fft result
void fftResult(AudioFFTBase &fft) {
    float *magnitudes = fft.magnitudes();

    beat_loop_timestamp = millis();
    
    peakb0.add(magnitudes[0]);

    max_filter = FILTER_POSITIONS & ((max_filter << 1) | peakb0.getPeak());

    #if defined(ROBOX_LED_MOTOR_CONTROLLER)

    // beat detected within BEAT_MIN_INTERACTION_TIME
    if (max_filter == 0x1) {
      beat_timekeeper = millis();

      if (led_motor_controller.is_led_enabled()) {
        allRandom();
      }

      if (led_motor_controller.is_motor_enabled()) {
        // Serial.printf("motor move: %ld\n", millis());
        // beat_counter++;
        motor->random_move();
      }
    }

    // no beat detected, but we still trigger one manually
    if (millis() > beat_timekeeper + BEAT_MIN_INTERACTION_TIME) {
      beat_timekeeper = millis() + (rand() % DUMMY_BEAT_RANGE);

      // if (led_motor_controller.is_led_enabled()) {
      //   allRandom();
      // } else {
      //   led_clear();
      // }

      if (led_motor_controller.is_motor_enabled()) {
        motor->random_move();
      }
    }

    #else
    if (max_filter == 0x1) {
      allRandom();
    }
    #endif
    
    #if defined(BEAT_TELEMETRY)
    Serial.printf(">bin0:%.2f\n>maxf:%.2f\n>peak0:%.2f\n>filter0:%.2f\n",
        magnitudes[0],
        ((max_filter == 0x01) ? magnitudes[0] : 0),
        (peakb0.getPeak()*magnitudes[0]), 
        peakb0.getFilt(),
        peakb0.getEpsilon()
    );
    #endif

    // fft_counter++;

    // if (millis() > fft_timekeeper) {
    //   Serial.printf("fft beat [%ld] %ld/s\n", millis(), fft_counter);

    //   fft_counter = 0;
    //   fft_timekeeper = millis() + 1000;
    // }

    // if (millis() > beat_timekeeper) {
    //   Serial.printf("count beat [%ld] %ld/s\n", millis(), beat_counter);

    //   beat_counter = 0;
    //   beat_timekeeper = millis() + 1000;
    // }
}


void fft_beat_setup(int samplerate) {
  // fft_counter = 0;
  // fft_timekeeper = millis() + 1000;

  // beat_counter = 0;
  // beat_timekeeper = millis() + 1000;

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

}

void led_clear() {
  FastLED.clear(true);
}

void led_init() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  led_clear();
}


uint32_t previousMillis = 0;
uint32_t interval = 5; // Update interval (milliseconds)
float brightness = 0.0;
float increment = 0.5; // How much to change the brightness by each update
  
void led_breath(bool alternate, LedBreathColors color) {
  uint32_t currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    brightness += increment;
    if (brightness <= 0 || brightness >= 255) {
      increment = -increment; // Reverse the direction of brightness change
    }

    uint8_t hue, sat;

    switch (color)
    {
    case r_red:
      hue = 0;
      sat = 255;
      break;

    case r_green:
      hue = 85;
      sat = 255;
      break;

    case r_blue:    
    default:
      hue = 160;
      sat = 255;
      break;
    }

     // Set brightness for LED 0
    leds[0] = CHSV(hue, sat, (int)brightness); // Blue color for LED 0
    
    if (alternate == false) {
      // Set brightness for LED 1 with synced brightness
      leds[1] = CHSV(hue, sat, (int)brightness); // Blue color for LED 1 with synced brightness
    }
    else {
      // Set brightness for LED 1 with inverted brightness
      leds[1] = CHSV(hue, sat, 255 - (int)brightness); // Blue color for LED 1 with inverted brightness
    }


    FastLED.show();
  }
}
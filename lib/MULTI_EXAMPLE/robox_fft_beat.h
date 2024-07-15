#pragma once

// #ifndef ROBOX_FFT_BEAT_H
// #define ROBOX_FFT_BEAT_H

#include "AudioTools.h"
#include "AudioLibs/AudioRealFFT.h" // or any other supported inplementation
#include "general_definitions.h"

// #define BEAT_TELEMETRY

#define NUM_LEDS 2
#define FILTER_POSITIONS 0b00111111     // determines how many bits we use in the shift register

extern AudioRealFFT fft;

void fft_beat_setup(int samplerate = 44100);

// #endif  // ROBOX_FFT_BEAT_H
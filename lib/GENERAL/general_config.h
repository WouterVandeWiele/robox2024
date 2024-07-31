#ifndef GENERAL_CONFIG_H
#define GENERAL_CONFIG_H

// #define DEFAULT_BUFFER_SIZE 4096
// #define DEFAULT_BUFFER_SIZE 50000

#define VOLUME_STEP 0.05


#define IO_EXPANDER

// LCD configuration
#define LCD_RUN_THREADED

// fft beat detection teleplot telemetry
// #define BEAT_TELEMETRY


#define PRIORITY_AUDIO_TASK 6
#define PRIORITY_LCD_TASK 7
#define PRIORITY_ADC_BUTTON 6
#define PRIORITY_BATTERY_MONITOR 6
#define PRIORITY_MOTOR 7

#define ADC_BATTERY_CONVERSION 2

#endif  // GENERAL_CONFIG_H
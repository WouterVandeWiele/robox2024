#ifndef GENERAL_CONFIG_H
#define GENERAL_CONFIG_H

#define RW_MODE false
#define RO_MODE true

// #define DEFAULT_BUFFER_SIZE 4096
// #define DEFAULT_BUFFER_SIZE 50000

#define VOLUME_STEP 0.05f

#define MAX_LED_BRIGHTNESS 1.0f
#define FIXED_MOTOR_SPEED 0.3f
#define BEAT_MIN_INTERACTION_TIME 1000
#define DUMMY_BEAT_RANGE 1000


#define IO_EXPANDER

// LCD configuration
#define LCD_RUN_THREADED

// fft beat detection teleplot telemetry
// #define BEAT_TELEMETRY
// #define BATTERY_TELEMETRY


#define PRIORITY_AUDIO_TASK 6
#define PRIORITY_LCD_TASK 7
#define PRIORITY_ADC_BUTTON 6
#define PRIORITY_BATTERY_MONITOR 6

#define ADC_BATTERY_CONVERSION 2
#define BATTERY_VERYLOWVOLTAGE 3200
#define BATTERY_LOWVOLTAGE 3400

#define GEM_PLAY_MENU 7   // go back to the play menu

#define BUTTON_SHORT_LONG_TH 500

// #define UPDATE_CHECK
// #define DEBUG_MOTOR_MOVE_WHILE_PLUGIN
#define DEBUG_WIFI_ENABLE_NO_SOURCE
#define ENABLE_WEB_CONFIGURATION_SERVER
// #define DEBUG_RESET_WIFI_CREDENTIALS_AT_START

#define LANG_NL
// #define LANG_EN

#endif  // GENERAL_CONFIG_H

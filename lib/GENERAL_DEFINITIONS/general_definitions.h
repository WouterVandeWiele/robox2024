#ifndef GENERAL_DEFINITIONS_H
#define GENERAL_DEFINITIONS_H

// ESP32 devkit
#ifdef PLATFORM_ESP32DEV
#define I2S_PIN_MUTE 22
#define I2S_PIN_DATA 32
#define I2S_PIN_BCLK 33
#define I2S_PIN_WS 25

#define LCDA0 26
#define LCDRW 27
#define LCDENABLE 13
// MSB . . . . . . LSB
#define LCDDATAPINS {19,23,18,5,17,16,4,0}
#endif

#ifdef PLATFORM_FREENOVE

// ESP32 FREENOVE S3
#define I2S_PIN_MUTE 2
#define I2S_PIN_DATA 1
#define I2S_PIN_BCLK 42
#define I2S_PIN_WS 41

#define LCDA0 4
#define LCDRW 10
#define LCDENABLE 5
// MSB . . . . . . LSB
#define LCDDATAPINS {9,6,8,7,18,15,17,16}

#endif

#ifdef PLATFORM_DEVKITM1

// ESP32 Devkit M1
#define I2S_PIN_MUTE 10
#define I2S_PIN_DATA 32
#define I2S_PIN_BCLK 33
#define I2S_PIN_WS 25

#define LCDA0 2
#define LCDRW 21
#define LCDENABLE 0
// MSB . . . . . . LSB
#define LCDDATAPINS {22,4,19,9,23,10,18,5}

#endif

#ifdef PLATFORM_ESP32DA

// ESP32 Devkit M1
#define I2S_PIN_MUTE 8
#define I2S_PIN_DATA 0
// #define I2S_PIN_BCLK 2
#define I2S_PIN_BCLK 13
#define I2S_PIN_WS 16

// #define LCDA0 2
// #define LCDRW 21
// #define LCDENABLE 0
// // MSB . . . . . . LSB
// #define LCDDATAPINS {22,4,19,9,23,10,18,5}

#endif

#define LOG_MAIN_TAG "Robox Main"
#define LOG_BLE_TAG "Robox BLE"
#define LOG_MUX_TAG "Robox MUX"
#define LOG_I2S_TAG "Robox I2S"
#define LOG_SD_TAG "Robox SD"
#define LOG_WEB_TAG "Robox Web"

#endif  // GENERAL_DEFINITIONS_H
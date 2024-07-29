#ifndef GENERAL_DEFINITIONS_H
#define GENERAL_DEFINITIONS_H

#include "stdint.h"

#ifdef PLATFORM_ROBOX_HW

// #define I2S_PIN_MUTE 10
// #define I2S_PIN_DATA 32
// #define I2S_PIN_BCLK 33
// #define I2S_PIN_WS 25

#define I2S_PIN_MUTE 2
#define I2S_PIN_DATA 32
#define I2S_PIN_BCLK 33
#define I2S_PIN_WS 25

#define IO_RESET 12
#define IO_INTERRUPT 39
#define I2C_SCL 27
#define I2C_SDA 26
#define I2C_SPEED 400000UL
// #define I2C_SPEED 10000
// #define I2C_SPEED 200000UL
#define I2C_PORT I2C_NUM_0
#define WRITE_BIT I2C_MASTER_WRITE  /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ    /*!< I2C master read */
#define ACK_CHECK_EN 0x1            /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0           /*!< I2C master will not check ack from slave */
#define ACK_VAL I2C_MASTER_ACK      /*!< I2C ack value */
#define NACK_VAL I2C_MASTER_NACK    /*!< I2C nack value */

#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */

#define LCD_DIS_PWR 14

#define LCD_A0 (uint8_t)0x20
#define LCD_RW (uint8_t)0x40
#define LCD_EN (uint8_t)0x80

#define LCD_DATA_PINS (uint8_t)0xFF

#define LCD_DATA_PORT 0
#define LCD_CONTROL_PORT 1

// Address of the IO expander
#define IO_EXPANDER_W_ADDRESS 0x20
#define IO_EXPANDER_R_ADDRESS 0xa0


// WS2812 pin
#define LED_PIN 15

// input buttons
#define BUTTON_1AC 34
#define BUTTON_1BD 35
#define BUTTON_12C 36
#define BUTTON_2AC 37
#define BUTTON_2BD 38

#define BATTERY_ADC 13

#define KEY_TH_1 825    // 0,825mV
#define KEY_TH_2 1885   // 1,885mV
// #define KEY_TC_C 2475   // 2,475mV

#define KEY_DEBOUNCE_DELAY 12

// motor pins

#define MOTOR_1_DIR 9
#define MOTOR_2_DIR 10
#define MOTOR_1_SPD 22
#define MOTOR_2_SPD 21
#define MOTOR_PWM_FREQ 10000
#define MOTOR_TIMER_BITS 12

#define MOTOR_EN 0x04
#define MOTOR_STANDBY 0x08

// battery pins
#define BATTERY_CHARGE 0x01
#define BATTERY_STANDBY 0x02


#endif


#define LOG_MAIN_TAG "Robox Main"
#define LOG_BLE_TAG "Robox BLE"
#define LOG_MUX_TAG "Robox MUX"
#define LOG_I2S_TAG "Robox I2S"
#define LOG_I2C_TAG "Robox I2C"
#define LOG_SD_TAG "Robox SD"
#define LOG_WEB_TAG "Robox Web"
#define LOG_LCD_TAG "Robox Lcd"
#define LOG_I2C_SCAN_TAG "Robox I2C Scanner"

#endif  // GENERAL_DEFINITIONS_H
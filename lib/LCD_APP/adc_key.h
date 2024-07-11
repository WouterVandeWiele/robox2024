#ifndef ADC_KEY_H
#define ADC_KEY_H

#include <Arduino.h>
// #include "lcd_config.h"
#include <GEM_adafruit_gfx.h>
#include "general_definitions.h"

#define VOLUME_UP 7
#define VOLUME_DOWN 8
#define PLAY_PAUSE 9
#define MOTOR_STOP 10

extern QueueHandle_t xQueueButtons;

enum threshold {TH0, TH1, TH2, PRESSED};

byte compare(uint32_t voltage, threshold &previous, byte button1, byte button2);
void adc_key_loop(void* parameter);
void adc_key_setup();

#endif // ADC_KEY_H
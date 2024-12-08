#ifndef ADC_KEY_H
#define ADC_KEY_H

#include <list>
#include <Arduino.h>
// #include "lcd_config.h"
#include <GEM_adafruit_gfx.h>
#include "general_definitions.h"

#define VOLUME_UP 7
#define VOLUME_DOWN 8
#define PLAY_PAUSE 9
#define MOTOR_STOP 10
#define AUDIO_NEXT 11
#define AUDIO_PREVIOUS 12

extern QueueHandle_t xQueueButtons;

enum threshold {TH0, TH1, TH2, PRESSED, LONG_PRESSED};

typedef struct ButtonPress{
    uint8_t button;
    bool long_press;
    uint32_t press_start_time;
    bool last_event;
} ButtonPress;

void compare(uint32_t voltage, std::list<threshold> &buffer, ButtonPress &result, byte button1, byte button2);
void adc_key_task(void* parameter);
void adc_key_setup();

#endif // ADC_KEY_H
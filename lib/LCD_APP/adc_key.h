#include <Arduino.h>
// #include "lcd_config.h"
#include <GEM_adafruit_gfx.h>
#include "general_definitions.h"

#define VOLUME_UP 7
#define VOLUME_DOWN 8
#define PLAY_PAUSE 9
#define MOTOR_STOP 10


static TaskHandle_t AdcKeyTaskHandle;

extern GEM_adafruit_gfx menu;   // should not be an extern but imported from lcd_config.h

enum threshold {TH0, TH1, TH2, PRESSED};

static threshold v_1ac_prev = TH0;
static threshold v_1bd_prev = TH0;
static threshold v_12c_prev = TH0;
static threshold v_2ac_prev = TH0;
static threshold v_2bd_prev = TH0;

byte compare(uint32_t voltage, threshold &previous, byte button1, byte button2) {
    threshold current;

    // detect state based on voltage
    if (voltage < KEY_TH_1) {
        current = TH0;
        previous = TH0;
    } else if ((voltage < KEY_TH_2) && (previous != PRESSED)) {
        current = TH1;
    } else if ((voltage >= KEY_TH_2) && (previous != PRESSED)) {
        current = TH2;
    }

    // detect first button press or button is still pressed
    if ((current == previous) || (previous == PRESSED)) {
        previous = PRESSED;
        byte b = (current == TH1) ? button1 : button2;
        Serial.printf("pressed, key: %d", b);
        return b;
    }

    return GEM_KEY_NONE;
    
}


void adc_key_loop(void* parameter) {
    while (true) {
        byte trigger = GEM_KEY_NONE;

        // GEM MENU
        if (menu.readyForKey()) {
            // up & down
            menu.registerKeyPress(
                compare(analogReadMilliVolts(BUTTON_1AC), v_1ac_prev, GEM_KEY_UP, GEM_KEY_DOWN)
            );
            // left & right
            menu.registerKeyPress(
                compare(analogReadMilliVolts(BUTTON_1BD), v_1bd_prev, GEM_KEY_RIGHT, GEM_KEY_LEFT)
            );

            // ok & cancel
            menu.registerKeyPress(
                compare(analogReadMilliVolts(BUTTON_12C), v_12c_prev, GEM_KEY_OK, GEM_KEY_CANCEL)
            );
        }
        
        byte b_2ac = compare(analogReadMilliVolts(BUTTON_2AC), v_2ac_prev, VOLUME_UP, VOLUME_DOWN);
        byte b_2bd = compare(analogReadMilliVolts(BUTTON_2BD), v_2bd_prev, PLAY_PAUSE, MOTOR_STOP);

        delay(KEY_DEBOUNCE_DELAY);
    }
}

void adc_key_setup() {
    // Push-buttons pin modes
    pinMode(BUTTON_1AC, INPUT);
    pinMode(BUTTON_1BD, INPUT);
    pinMode(BUTTON_12C, INPUT);
    pinMode(BUTTON_2AC, INPUT);
    pinMode(BUTTON_2BD, INPUT);

    //set the resolution to 12 bits (0-4095)
    analogReadResolution(12);

    xTaskCreate(
        adc_key_loop,       //Function to implement the task 
        "adc_key_task", //Name of the task
        6000,       //Stack size in words 
        NULL,       //Task input parameter 
        0,          //Priority of the task 
        &AdcKeyTaskHandle       //Task handle.
    );
}


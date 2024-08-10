#include <list>
#include "general_config.h"
#include "adc_key.h"
#include "lcd_screen.h"

#include "robox_led_motor_controller.h"
extern LedMotorController led_motor_controller;

#include "robox_audio_mux.h"
extern RoboxAudioMux mux;

static TaskHandle_t AdcKeyTaskHandle;

static std::list<threshold> v_1ac_prev = {TH0, TH0, TH0};
static std::list<threshold> v_1bd_prev = {TH0, TH0, TH0};
static std::list<threshold> v_12c_prev = {TH0, TH0, TH0};
static std::list<threshold> v_2ac_prev = {TH0, TH0, TH0};
static std::list<threshold> v_2bd_prev = {TH0, TH0, TH0};

static ButtonPress b_1ac = {GEM_KEY_NONE, false, 0, false};
static ButtonPress b_1bd = {GEM_KEY_NONE, false, 0, false};
static ButtonPress b_12c = {GEM_KEY_NONE, false, 0, false};
static ButtonPress b_2ac = {GEM_KEY_NONE, false, 0, false};
static ButtonPress b_2bd = {GEM_KEY_NONE, false, 0, false};

QueueHandle_t xQueueButtons;


void compare(uint32_t voltage, std::list<threshold> &buffer, ButtonPress &result, byte button1, byte button2) {
    result.button = GEM_KEY_NONE;
    result.long_press = false;
    result.last_event = false;

    // Serial.printf("\n>voltage:%d\n", voltage);

    // detect state based on voltage
    threshold first_element = TH0;
    if (voltage < KEY_TH_1) {
        buffer.emplace_front(TH0);
        first_element = TH0;
    } else if (voltage < KEY_TH_2) {
        buffer.emplace_front(TH1);
        first_element = TH1;
    } else if (voltage >= KEY_TH_2) {
        buffer.emplace_front(TH2);
        first_element = TH2;
    }

    buffer.pop_back();

    // Serial.printf("\n>current:%d\n>previous:%d\n", current, previous);
    uint8_t count_TH1 = 0;
    uint8_t count_TH2 = 0;
    threshold last_element = TH0;

    // Serial.print("vector: ");
    for (std::list<threshold>::iterator  it=buffer.begin(); it != buffer.end(); ++it) {
        if (*it == TH1) count_TH1++;
        if (*it == TH2) count_TH2++;
        last_element = *it;
        // Serial.printf("%d ", *it);
    }
    // Serial.println("");

    if ((((count_TH1 == 2) && (first_element != TH0) )|| ((count_TH2 == 2) && (first_element != TH0))) ) {
        // first TH event
        result.press_start_time = millis();
        result.button = (buffer.front() == TH1) ? button1 : button2;
        // result.button = (buffer.front() == TH1) ? button1 : button2;
        Serial.println("First button event");
    }
    else if ((((count_TH1 == 0) && (count_TH2 == 1)) || ((count_TH1 == 1) && (count_TH2 == 0))) && (last_element != TH0)) {
        // last TH event
        result.press_start_time = millis() - result.press_start_time;
        result.long_press = (result.press_start_time > BUTTON_SHORT_LONG_TH) ? true : false;
        result.last_event = true;
        result.button = (last_element == TH1) ? button1 : button2;
        Serial.println("Last button event");
    }

    // Serial.printf("\nbuf len: %d, count TH1: %d, count TH2: %d, button: %d, long press %d\n", buffer.size(), count_TH1, count_TH2, result.button, result.long_press ? 1 : 0);
}

void adc_empty_key() {
    ButtonPress b_empty = {
        .button = GEM_KEY_NONE,
        .long_press = false,
        .press_start_time = 0,
        .last_event = true,
    };
    xQueueSend(xQueueButtons, &b_empty, 0);
}

[[noreturn]] void adc_key_task(void* parameter) {
    while (true) {
        byte trigger = GEM_KEY_NONE;

        compare(analogReadMilliVolts(BUTTON_1AC), v_1ac_prev, b_1ac, GEM_KEY_RIGHT, GEM_KEY_LEFT);
        if (b_1ac.last_event) {

            Serial.printf("\n>key_%d:%d long press: %d time: %ld last event: %d\n", BUTTON_1AC, b_1ac.button, b_1ac.long_press ? 1 : 0, b_1ac.press_start_time, b_1ac.last_event ? 1 : 0);
            xQueueSend(xQueueButtons, &b_1ac, 0);
        }

        compare(analogReadMilliVolts(BUTTON_1BD), v_1bd_prev, b_1bd, GEM_KEY_UP, GEM_KEY_DOWN);
        if (b_1bd.last_event) {
            Serial.printf("\n>key_%d:%d long press: %d time: %ld last event: %d\n", BUTTON_1BD, b_1bd.button, b_1bd.long_press ? 1 : 0, b_1bd.press_start_time, b_1bd.last_event ? 1 : 0);
            xQueueSend(xQueueButtons, &b_1bd, 0);
        }

        compare(analogReadMilliVolts(BUTTON_12C), v_12c_prev, b_12c, GEM_KEY_OK, PLAY_PAUSE);
        if (b_12c.last_event) {
            Serial.printf("\n>key_%d:%d long press: %d time: %ld last event: %d\n", BUTTON_12C, b_12c.button, b_12c.long_press ? 1 : 0, b_12c.press_start_time, b_12c.last_event ? 1 : 0);
            
            if (b_12c.button == GEM_KEY_OK) {
                xQueueSend(xQueueButtons, &b_12c, 0);
            }
            if (b_12c.button == PLAY_PAUSE) {
                if (b_12c.long_press == false) {
                    mux.audio_play_pause();
                }
                else {
                    led_motor_controller.next();
                    lcd_invalidate(INVALIDATE_ALL);
                }
            }
        }
        
        compare(analogReadMilliVolts(BUTTON_2AC), v_2ac_prev, b_2ac, AUDIO_NEXT, AUDIO_PREVIOUS);
        if (b_2ac.last_event) {
            Serial.printf("\n>key_%d:%d long press: %d time: %ld last event: %d\n", BUTTON_2AC, b_2ac.button, b_2ac.long_press ? 1 : 0, b_2ac.press_start_time, b_2ac.last_event ? 1 : 0);
            // xQueueSend(xQueueButtons, &b_2ac, 0);
            if ((b_2ac.button == AUDIO_NEXT) && (b_2ac.long_press == false)) {
                mux.audio_next();
            }
            if ((b_2ac.button == AUDIO_PREVIOUS) && (b_2ac.long_press == false)) {
                mux.audio_previous();
            }
        }

        compare(analogReadMilliVolts(BUTTON_2BD), v_2bd_prev, b_2bd, VOLUME_UP, VOLUME_DOWN);
        if (b_2bd.last_event) {
            Serial.printf("\n>key_%d:%d long press: %d time: %ld last event: %d\n", BUTTON_2BD, b_2bd.button, b_2bd.long_press ? 1 : 0, b_2bd.press_start_time, b_2bd.last_event ? 1 : 0);
            // xQueueSend(xQueueButtons, &b_2bd, 0);
            if ((b_2bd.button == VOLUME_UP) && (b_2bd.long_press == false)) {
                mux.volume_increment();
                lcd_invalidate(INVALIDATE_VOLUME);
            }
            if ((b_2bd.button == VOLUME_DOWN) && (b_2bd.long_press == false)) {
                mux.volume_decrement();
                lcd_invalidate(INVALIDATE_VOLUME);
            }
            
        }

        delay(KEY_DEBOUNCE_DELAY);
    }
}

void adc_key_setup() {

    // create button queue
    xQueueButtons = xQueueCreate(10, sizeof(ButtonPress));

    // set the resolution to 12 bits (0-4095)
    analogReadResolution(12);

    xTaskCreatePinnedToCore(
        adc_key_task,       //Function to implement the task 
        "adc_key_task", //Name of the task
        3000,       //Stack size in words 
        NULL,       //Task input parameter 
        PRIORITY_ADC_BUTTON,          //Priority of the task 
        &AdcKeyTaskHandle,       //Task handle.
        1           // Core you want to run the task on (0 or 1)
    );
}

#include <list>
#include "general_config.h"
#include "adc_key.h"

#include "robox_audio_mux.h"
extern RoboxAudioMux mux;

static TaskHandle_t AdcKeyTaskHandle;

static std::list<threshold> v_1ac_prev = {TH0, TH0, TH0};
static std::list<threshold> v_1bd_prev = {TH0, TH0, TH0};
static std::list<threshold> v_12c_prev = {TH0, TH0, TH0};
static std::list<threshold> v_2ac_prev = {TH0, TH0, TH0};
static std::list<threshold> v_2bd_prev = {TH0, TH0, TH0};

static uint32_t s_1ac = 0;
static uint32_t s_1bd = 0;
static uint32_t s_12c = 0;
static uint32_t s_2ac = 0;
static uint32_t s_2bd = 0;

static uint32_t t_1ac = 0;
static uint32_t t_1bd = 0;
static uint32_t t_12c = 0;
static uint32_t t_2ac = 0;
static uint32_t t_2bd = 0;

QueueHandle_t xQueueButtons;


ButtonPress compare(uint32_t voltage, std::list<threshold> &buffer, byte button1, byte button2) {
    // threshold current;
    ButtonPress result = {
        .button = GEM_KEY_NONE,
        .long_press = false,
        .press_time = 0,
    };

    // Serial.printf("\n>voltage:%d\n", voltage);

    // detect state based on voltage
    if (voltage < KEY_TH_1) {
        buffer.emplace_front(TH0);
    } else if (voltage < KEY_TH_2) {
        buffer.emplace_front(TH1);
    } else if (voltage >= KEY_TH_2) {
        buffer.emplace_front(TH2);
    }

    buffer.pop_back();

    // Serial.printf("\n>current:%d\n>previous:%d\n", current, previous);
    uint8_t count_TH1 = 0;
    uint8_t count_TH2 = 0;
    uint8_t last_element = 0;
    for (std::list<threshold>::reverse_iterator  it=buffer.rbegin(); it != buffer.rend(); ++it) {
        if (*it == TH1) count_TH1++;
        if (*it == TH2) count_TH2++;
        last_element = *it;
        // Serial.printf("%d ", *it);
    }

    if ((count_TH1 == 3) || (count_TH2 == 3)) {
        result.long_press = true;
        result.button = (buffer.front() == TH1) ? button1 : button2;
    }
    else if (((count_TH1 == 2) || (count_TH2 == 2)) && (last_element != TH0)) {
        result.button = (buffer.front() == TH1) ? button1 : button2;
    }

    // Serial.printf("\nbuf len: %d, count TH1: %d, count TH2: %d, button: %d, long press %d\n", buffer.size(), count_TH1, count_TH2, result.button, result.long_press ? 1 : 0);
    return result;
    
}


void adc_key_loop(void* parameter) {
    while (true) {
        byte trigger = GEM_KEY_NONE;

        ButtonPress b_1ac = compare(analogReadMilliVolts(BUTTON_1AC), v_1ac_prev, GEM_KEY_RIGHT, GEM_KEY_LEFT);
        if (b_1ac.button) {
            if (b_1ac.long_press == 0) s_1ac = millis();
            else t_1ac = millis() - s_1ac;
            b_1ac.press_time = t_1ac;

            Serial.printf("\n>key_%d:%d long press: %d time: %ld\n", BUTTON_1AC, b_1ac.button, b_1ac.long_press ? 1 : 0, t_1ac);
            xQueueSend(xQueueButtons, &b_1ac, 0);
        }
        else {
            s_1ac = 0;
            t_1ac = 0;
        }

        ButtonPress b_1bd = compare(analogReadMilliVolts(BUTTON_1BD), v_1bd_prev, GEM_KEY_UP, GEM_KEY_DOWN);
        if (b_1bd.button){
            if (b_1bd.long_press == 0) s_1bd = millis();
            else t_1bd = millis() - s_1bd;
            b_1bd.press_time = t_1bd;

            Serial.printf("\n>key_%d:%d long press: %d time: %ld\n", BUTTON_1BD, b_1bd.button, b_1bd.long_press ? 1 : 0, t_1bd);
            xQueueSend(xQueueButtons, &b_1bd, 0);
        }
        else {
            s_1bd = 0;
            t_1bd = 0;
        }

        ButtonPress b_12c = compare(analogReadMilliVolts(BUTTON_12C), v_12c_prev, GEM_KEY_OK, PLAY_PAUSE);
        if (b_12c.button) {
            if (b_12c.long_press == 0) s_12c = millis();
            else t_12c = millis() - s_12c;
            b_12c.press_time = t_12c;

            Serial.printf("\n>key_%d:%d long press: %d time: %ld\n", BUTTON_12C, b_12c.button, b_12c.long_press ? 1 : 0, t_12c);
            
            if (b_12c.button == GEM_KEY_OK) {
                xQueueSend(xQueueButtons, &b_12c, 0);
            }
            if ((b_12c.button == PLAY_PAUSE) && (b_12c.long_press == false)) {
                mux.audio_play_pause();
            }
        }
        else {
            s_12c = 0;
            t_12c = 0;
        }
        
        ButtonPress b_2ac = compare(analogReadMilliVolts(BUTTON_2AC), v_2ac_prev, AUDIO_NEXT, AUDIO_PREVIOUS);
        if (b_2ac.button) {
            if (b_2ac.long_press == 0) s_2ac = millis();
            else t_2ac = millis() - s_2ac;
            b_2ac.press_time = t_2ac;

            Serial.printf("\n>key_%d:%d long press: %d time: %ld\n", BUTTON_2AC, b_2ac.button, b_2ac.long_press ? 1 : 0, t_2ac);
            // xQueueSend(xQueueButtons, &b_2ac, 0);
            if ((b_2ac.button == AUDIO_NEXT) && (b_2ac.long_press == false)) {
                mux.audio_next();
            }
            if ((b_2ac.button == AUDIO_PREVIOUS) && (b_2ac.long_press == false)) {
                mux.audio_previous();
            }
        }
        else {
            s_2ac = 0;
            t_2ac = 0;
        }

        ButtonPress b_2bd = compare(analogReadMilliVolts(BUTTON_2BD), v_2bd_prev, VOLUME_UP, VOLUME_DOWN);
        if (b_2bd.button) {
            if (b_2bd.long_press == 0) s_2bd = millis();
            else t_2bd = millis() - s_2bd;
            b_2bd.press_time = t_2bd;

            Serial.printf("\n>key_%d:%d long press: %d time: %ld\n", BUTTON_2BD, b_2bd.button, b_2bd.long_press ? 1 : 0, t_2bd);
            // xQueueSend(xQueueButtons, &b_2bd, 0);
            if ((b_2bd.button == VOLUME_UP) && (b_2bd.long_press == false)) {
                mux.volume_increment();
            }
            if ((b_2bd.button == VOLUME_DOWN) && (b_2bd.long_press == false)) {
                mux.volume_decrement();
            }
            
        }
        else {
            s_2bd = 0;
            t_2bd = 0;
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
        adc_key_loop,       //Function to implement the task 
        "adc_key_task", //Name of the task
        6000,       //Stack size in words 
        NULL,       //Task input parameter 
        PRIORITY_ADC_BUTTON,          //Priority of the task 
        &AdcKeyTaskHandle,       //Task handle.
        1           // Core you want to run the task on (0 or 1)
    );
}
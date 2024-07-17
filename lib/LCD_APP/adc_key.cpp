#include <list>
#include "general_config.h"
#include "adc_key.h"

static TaskHandle_t AdcKeyTaskHandle;

static std::list<threshold> v_1ac_prev = {TH0, TH0, TH0};
static std::list<threshold> v_1bd_prev = {TH0, TH0, TH0};
static std::list<threshold> v_12c_prev = {TH0, TH0, TH0};
static std::list<threshold> v_2ac_prev = {TH0, TH0, TH0};
static std::list<threshold> v_2bd_prev = {TH0, TH0, TH0};

QueueHandle_t xQueueButtons;


ButtonPress compare(uint32_t voltage, std::list<threshold> &buffer, byte button1, byte button2) {
    // threshold current;
    ButtonPress result = {
        .button = GEM_KEY_NONE,
        .long_press = false,
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

        ButtonPress b_1ac = compare(analogReadMilliVolts(BUTTON_1AC), v_1ac_prev, GEM_KEY_UP, GEM_KEY_DOWN);
        if (b_1ac.button) {
            Serial.printf("\n>key_%d:%d long press: %d\n", BUTTON_1AC, b_1ac.button, b_1ac.long_press ? 1 : 0);
            xQueueSend(xQueueButtons, &b_1ac, 0);
        }

        ButtonPress b_1bd = compare(analogReadMilliVolts(BUTTON_1BD), v_1bd_prev, GEM_KEY_RIGHT, GEM_KEY_LEFT);
        if (b_1bd.button){
            Serial.printf("\n>key_%d:%d long press: %d\n", BUTTON_1BD, b_1bd, b_1bd.long_press ? 1 : 0);
            xQueueSend(xQueueButtons, &b_1bd, 0);
        }

        ButtonPress b_12c = compare(analogReadMilliVolts(BUTTON_12C), v_12c_prev, GEM_KEY_OK, GEM_KEY_CANCEL);
        if (b_12c.button) {
            Serial.printf("\n>key_%d:%d long press: %d\n", BUTTON_12C, b_12c, b_12c.long_press ? 1 : 0);
            xQueueSend(xQueueButtons, &b_12c, 0);
        }
        
        ButtonPress b_2ac = compare(analogReadMilliVolts(BUTTON_2AC), v_2ac_prev, VOLUME_UP, VOLUME_DOWN);
        if (b_2ac.button) {
            Serial.printf("\n>key_%d:%d long press: %d\n", BUTTON_2AC, b_2ac, b_2ac.long_press ? 1 : 0);
            xQueueSend(xQueueButtons, &b_2ac, 0);
        }

        ButtonPress b_2bd = compare(analogReadMilliVolts(BUTTON_2BD), v_2bd_prev, PLAY_PAUSE, MOTOR_STOP);
        if (b_2bd.button) {
            Serial.printf("\n>key_%d:%d long press: %d\n", BUTTON_2BD, b_2bd, b_2bd.long_press ? 1 : 0);
            xQueueSend(xQueueButtons, &b_2bd, 0);
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
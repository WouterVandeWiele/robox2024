#include <Arduino.h>
#include "robox_battery.h"
#include "general_definitions.h"
#include "general_config.h"

static uint32_t _battery_voltage = 0;
static TaskHandle_t batteryTaskHandle;


void battery_loop(void* parameter) {
    uint32_t raw_adc = 0;

    while (true) {
        raw_adc = analogReadMilliVolts(BATTERY_ADC);
        raw_adc *= ADC_BATTERY_CONVERSION;
        _battery_voltage = raw_adc;

        delay(30000);
    }
}


RoboxBattery::RoboxBattery(RoboxIoExpander* io): io(io) {
    xTaskCreatePinnedToCore(
        battery_loop,       //Function to implement the task 
        "battery_task", //Name of the task
        1000,       //Stack size in words 
        NULL,       //Task input parameter 
        PRIORITY_BATTERY_MONITOR,          //Priority of the task 
        &batteryTaskHandle,       //Task handle.
        1           // Core you want to run the task on (0 or 1)
    );
}

ExpanderConfig RoboxBattery::io_config() {
    ExpanderConfig config = RoboxIoExpander::get_default_config();

    config.out_state[LCD_CONTROL_PORT] = (BATTERY_CHARGE | BATTERY_STANDBY);

    // config.out_config[LCD_CONTROL_PORT] = ~(MOTOR_EN | MOTOR_STANDBY);

    config.interrupts[LCD_CONTROL_PORT] = (BATTERY_CHARGE | BATTERY_STANDBY);

    return config;
}

uint32_t RoboxBattery::battery_voltage() {
    return _battery_voltage;
}

void RoboxBattery::io_interrupt_observer(std::vector<uint8_t>& data) {
    // RoboxLcdScreen * self = static_cast<RoboxLcdScreen*>(this_pointer);

    // implement callback code when an interrupt is generated
}
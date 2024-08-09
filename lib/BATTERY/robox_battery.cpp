#include <Arduino.h>
#include "robox_battery.h"
#include "general_definitions.h"
#include "general_config.h"
#include "robox_motor.h"
#include "robox_restart.h"

extern RoboxMotor* motor;

static uint32_t _battery_voltage = 0;
static TaskHandle_t batteryTaskHandle;
static uint8_t _batteryChargerStatusCharging = 0;
static uint8_t _batteryChargerStatusChgStBy = 0;

QueueHandle_t xQueueBattery;

typedef struct BatteryData{
  uint32_t battery_voltage;
  uint8_t batteryChargerStatusCharging;
  uint8_t batteryChargerStatusChgStBy;
  uint8_t batteryLow;
} BatteryData;

void battery_loop(void* parameter) {
    Serial.println("battery loop setup");

    uint32_t raw_adc = 0;
    uint8_t batteryChargerStatus = 0;
    RoboxIoExpander* io = (RoboxIoExpander*)parameter;
    BatteryData BatteryDataStatus = {
        .battery_voltage = 0,
        .batteryChargerStatusCharging = 0,
        .batteryChargerStatusChgStBy = 0,
        .batteryLow = BATTERYHIGH,
    };    
    xQueueBattery = xQueueCreate(1, sizeof(BatteryData));

    while (true) {
        Serial.printf("input0 %d\n", io->get_inputs(0));
        Serial.printf("input1 %d\n", io->get_inputs(1));
        batteryChargerStatus = io->get_inputs(LCD_CONTROL_PORT) & (BATTERY_CHARGE | BATTERY_STANDBY);
        _batteryChargerStatusCharging = batteryChargerStatus & BATTERY_CHARGE;
        _batteryChargerStatusChgStBy = (batteryChargerStatus & BATTERY_STANDBY) >> 1;

        raw_adc = analogReadMilliVolts(BATTERY_ADC);
        _battery_voltage = raw_adc * ADC_BATTERY_CONVERSION;

        Serial.printf(">battery_voltage:%ld\n", _battery_voltage);

        BatteryDataStatus.battery_voltage = _battery_voltage;
        BatteryDataStatus.batteryChargerStatusCharging = _batteryChargerStatusCharging;
        BatteryDataStatus.batteryChargerStatusChgStBy = _batteryChargerStatusChgStBy;
        BatteryDataStatus.battery_voltage = _battery_voltage;
        
        Serial.println("charger pins");
        Serial.println(_batteryChargerStatusChgStBy);
        Serial.println(_batteryChargerStatusCharging);

        if (_battery_voltage < BATTERYLOWVOLTAGE) {
            BatteryDataStatus.batteryLow = BATTERYLOW;
        }
        else if (_battery_voltage < BATTERYVERYLOWVOLTAGE) {
            BatteryDataStatus.batteryLow = BATTERYVERYLOW;
        }else {
            BatteryDataStatus.batteryLow = BATTERYHIGH;
        }
        if (_batteryChargerStatusChgStBy == 0 || _batteryChargerStatusCharging == 0 ) {    //if charging or if charging terminated
            motor->motorLowBattery(true);
            Serial.print("let's disable the motors\n");
        }else
        {
            motor->motorLowBattery(false);
            Serial.print("let's enable the motors\n");
        }
        xQueueSend(xQueueBattery, &BatteryDataStatus, 0);
        delay(1000);    //every 1seconds
    }
}

RoboxBattery::RoboxBattery(RoboxIoExpander* io): io(io) {
    
}

ExpanderConfig RoboxBattery::io_config() {
    ExpanderConfig config = RoboxIoExpander::get_default_config();

    config.out_state[LCD_CONTROL_PORT] = (BATTERY_CHARGE | BATTERY_STANDBY);

    // config.interrupts[LCD_CONTROL_PORT] = (BATTERY_CHARGE | BATTERY_STANDBY);

    return config;
}

void RoboxBattery::initBattery()
{
    Serial.println("battery setup");

    xTaskCreatePinnedToCore(
        battery_loop,       //Function to implement the task 
        "battery_task", //Name of the task
        3000,       //Stack size in words 
        (void *) io,       //Task input parameter 
        PRIORITY_BATTERY_MONITOR,          //Priority of the task 
        &batteryTaskHandle,       //Task handle.
        1           // Core you want to run the task on (0 or 1)
    );
}

uint32_t RoboxBattery::battery_voltage() {
    return _battery_voltage;
}

uint8_t RoboxBattery::batteryChargerStatusCharging() {
    return _batteryChargerStatusCharging;
}

uint8_t RoboxBattery::batteryChargerStatusChgStBy() {
    return _batteryChargerStatusChgStBy;
}

void RoboxBattery::io_interrupt_observer(std::vector<uint8_t>& data) {
    // RoboxLcdScreen * self = static_cast<RoboxLcdScreen*>(this_pointer);

    // implement callback code when an interrupt is generated
}
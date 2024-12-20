#include <Arduino.h>
#include "robox_battery.h"
#include "general_definitions.h"
#include "general_config.h"
#include "robox_motor.h"
#include "robox_restart.h"

extern RoboxMotor* motor;

// static uint32_t _battery_voltage = 0;
// static uint8_t _batteryChargerStatusCharging = 0;
// static uint8_t _batteryChargerStatusChgStBy = 0;

static BatteryData BatteryStatus = {
    .voltage = 0,
    .chargerCharging = 0,
    .chargerChgStBy = 0,
    .state = battery_high,
};   

static TaskHandle_t batteryTaskHandle;
QueueHandle_t xQueueBattery;

[[noreturn]] void battery_task(void* parameter) {
    Serial.println("battery loop setup");

    uint32_t raw_adc = 0;
    uint8_t batteryChargerStatus = 0;
    RoboxIoExpander* io = (RoboxIoExpander*)parameter;

    uint8_t charge_ind_short_count = 0;
    uint32_t charge_ind_long_count = 0;
    uint32_t charge_ind_timestamp = millis() + BATTERY_CHARGE_IND_INTERVAL;

    BatteryStatus.chargerChgStBy = true;

    while (true) {
        // Note: batteryChargerStatus is inverted since the outputs on the charging IC are active low
        batteryChargerStatus = io->get_inputs(LCD_CONTROL_PORT);

        // Serial.printf("port %d: %x\n", LCD_CONTROL_PORT, batteryChargerStatus);
        
        BatteryStatus.chargerCharging = (batteryChargerStatus & BATTERY_CHARGE) ? true : false;
        if (BatteryStatus.chargerCharging) {
            BatteryStatus.chargerChgStBy = true;
        }
        // BatteryStatus.chargerChgStBy = (batteryChargerStatus & BATTERY_STANDBY) ? true : false;
        if (millis() > charge_ind_timestamp) {
            charge_ind_timestamp = millis() + BATTERY_CHARGE_IND_INTERVAL;

            if (charge_ind_short_count > 45) {
                charge_ind_long_count += 1;
            }
            else {
                charge_ind_long_count = 0;
            }

            charge_ind_short_count = 0;
            
            if (charge_ind_long_count > 20) {
                BatteryStatus.chargerChgStBy = false;
            }
            else {
                BatteryStatus.chargerChgStBy = true;
            }
        }


        BatteryStatus.voltage = analogReadMilliVolts(BATTERY_ADC) * ADC_BATTERY_CONVERSION;

        #if defined(BATTERY_TELEMETRY)
        Serial.printf(">battery_voltage:%ld\n", BatteryStatus.voltage);
        Serial.printf(">battery_chg_stby: %d\n", BatteryStatus.chargerChgStBy ? 1 : 0);
        Serial.printf(">battery_chg_ind: %d\n", BatteryStatus.chargerCharging ? 1 : 0);
        Serial.printf(">charge_ind_short_count: %d\n", charge_ind_short_count);
        Serial.printf(">charge_ind_long_count: %d\n", charge_ind_long_count);
        #endif

        if (BatteryStatus.voltage > BATTERY_ABOVE_CHARGE_TH) {
            charge_ind_short_count++;
        }
        else if (BatteryStatus.voltage < BATTERY_VERYLOWVOLTAGE) {
            BatteryStatus.state = battery_verylow;
        }
        else if (BatteryStatus.voltage < BATTERY_LOWVOLTAGE) {
            BatteryStatus.state = battery_low;
        }
        else {
            BatteryStatus.state = battery_high;
        }

        if (BatteryStatus.chargerChgStBy == 0 || BatteryStatus.chargerCharging == 0 ) {    //if charging or if charging terminated
            #if !defined(DEBUG_MOTOR_MOVE_WHILE_PLUGIN)
            motor->motorLowBattery(true);
            #endif

            #if defined(BATTERY_TELEMETRY)
            Serial.print("let's disable the motors\n");
            #endif
        }
        else if ((BatteryStatus.state == battery_high) || (BatteryStatus.state == battery_low))
        {
            #if !defined(DEBUG_MOTOR_MOVE_WHILE_PLUGIN)
            motor->motorLowBattery(false);
            #endif

            #if defined(BATTERY_TELEMETRY)
            Serial.print("let's enable the motors\n");
            #endif
        }
        else {
            #if !defined(DEBUG_MOTOR_MOVE_WHILE_PLUGIN)
            motor->motorLowBattery(true);
            #endif

            #if defined(BATTERY_TELEMETRY)
            Serial.print("safe state disable the motors\n");
            #endif
        }
        xQueueSend(xQueueBattery, &BatteryStatus, 0);

        delay(1000);    //every second
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

    xQueueBattery = xQueueCreate(1, sizeof(BatteryData));

    xTaskCreatePinnedToCore(
        battery_task,       //Function to implement the task 
        "battery_task", //Name of the task
        3000,       //Stack size in words 
        (void *) io,       //Task input parameter 
        PRIORITY_BATTERY_MONITOR,          //Priority of the task 
        &batteryTaskHandle,       //Task handle.
        1           // Core you want to run the task on (0 or 1)
    );
}

uint32_t RoboxBattery::battery_voltage() {
    return BatteryStatus.voltage;
}

// uint8_t RoboxBattery::batteryChargerStatusCharging() {
//     return BatteryStatus.chargerCharging;
// }

// uint8_t RoboxBattery::batteryChargerStatusChgStBy() {
//     return BatteryStatus.chargerChgStBy;
// }

void RoboxBattery::io_interrupt_observer(std::vector<uint8_t>& data) {
    // RoboxLcdScreen * self = static_cast<RoboxLcdScreen*>(this_pointer);

    // implement callback code when an interrupt is generated
}
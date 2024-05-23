#include "robox_battery.h"


RoboxBattery::RoboxBattery(RoboxIoExpander* io): io(io) {

}

ExpanderConfig RoboxBattery::io_config() {
    ExpanderConfig config = RoboxIoExpander::get_default_config();

    config.out_state[LCD_CONTROL_PORT] = (BATTERY_CHARGE | BATTERY_STANDBY);

    // config.out_config[LCD_CONTROL_PORT] = ~(MOTOR_EN | MOTOR_STANDBY);

    config.interrupts[LCD_CONTROL_PORT] = (BATTERY_CHARGE | BATTERY_STANDBY);

    return config;
}

void RoboxBattery::io_interrupt_observer(std::vector<uint8_t>& data) {
    // RoboxLcdScreen * self = static_cast<RoboxLcdScreen*>(this_pointer);

    // implement callback code when an interrupt is generated
}
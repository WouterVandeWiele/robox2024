#include <Arduino.h>
#include <mutex>
#include "robox_motor.h"
#include "general_definitions.h"

#define MAX_12_BITS 4095


RoboxMotor::RoboxMotor(RoboxIoExpander* io): io(io), disbabledByLowBattery(false) {

}


void RoboxMotor::init() {

    // speed pins
    ledcAttachPin(MOTOR_1_SPD, 1); 
    ledcSetup(1, MOTOR_PWM_FREQ, MOTOR_TIMER_BITS);
    ledcAttachPin(MOTOR_2_SPD, 2);
    ledcSetup(2, MOTOR_PWM_FREQ, MOTOR_TIMER_BITS);

    // direction pins
    pinMode(MOTOR_1_DIR, OUTPUT);
    pinMode(MOTOR_2_DIR, OUTPUT);

    // io expander pins
}

void RoboxMotor::set_speed(float m1, float m2) {
    // m1 and m2 between 0 and 1

    ledcWrite(1, (uint32_t)(MAX_12_BITS * m1));
    ledcWrite(2, (uint32_t)(MAX_12_BITS * m2));
}

void RoboxMotor::set_direction(bool m1, bool m2) {
    digitalWrite(MOTOR_1_DIR, (m1) ? LOW : HIGH);
    digitalWrite(MOTOR_2_DIR, (m2) ? HIGH : LOW);
}

void RoboxMotor::enable(bool enable) {
    if(disbabledByLowBattery == false)
    {
        io->set_output(LCD_CONTROL_PORT, (MOTOR_EN | MOTOR_STANDBY), (MOTOR_EN | MOTOR_STANDBY));
    }
}

void RoboxMotor::shutdown(bool shutdown) {
    io->set_output(LCD_CONTROL_PORT, ~(MOTOR_EN | MOTOR_STANDBY), (MOTOR_EN | MOTOR_STANDBY));
}

void RoboxMotor::motorLowBattery(bool _disbabledByLowBattery) {
    if(_disbabledByLowBattery == true){
        shutdown(true);
    }
    disbabledByLowBattery = _disbabledByLowBattery;
}

ExpanderConfig RoboxMotor::io_config() {
    ExpanderConfig config = RoboxIoExpander::get_default_config();

    config.out_state[LCD_CONTROL_PORT] = ~(MOTOR_EN | MOTOR_STANDBY);

    config.out_config[LCD_CONTROL_PORT] = ~(MOTOR_EN | MOTOR_STANDBY);

    // config.interrupts[LCD_CONTROL_PORT] = <fault sense pin>

    return config;
}

void RoboxMotor::io_interrupt_observer(std::vector<uint8_t>& data) {
    // RoboxLcdScreen * self = static_cast<RoboxLcdScreen*>(this_pointer);

    // implement callback code when an interrupt is generated
}
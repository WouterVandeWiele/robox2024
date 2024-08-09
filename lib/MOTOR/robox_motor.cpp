#include <Arduino.h>
#include <mutex>
#include "robox_motor.h"
#include "general_config.h"
#include "general_definitions.h"
#include <random>

#define MAX_12_BITS 4095


RoboxMotor::RoboxMotor(RoboxIoExpander* io)
    : io(io)
    , disbabledByLowBattery(false)
    , is_motor_on(false)
    , last_random_move(0)
    {}


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
        is_motor_on = true;
    }
}

void RoboxMotor::shutdown(bool shutdown) {
    is_motor_on = false;
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

void RoboxMotor::shutdown_idempotent() {
    if (is_motor_on == true) {
        shutdown(true);
    }
}

void RoboxMotor::enable_idempotent() {
    if (is_motor_on == false) {
        enable(true);
    }
}

bool RoboxMotor::is_shutdown() {
    // stub
    return false;
}

void RoboxMotor::random_move() {

    if (millis() < (last_random_move + 400)) {
        return;
    }

    MotorMoves move = static_cast<MotorMoves>(rand() % mm_last);

    switch (move)
    {
    case mm_forward:
        set_speed(FIXED_MOTOR_SPEED, FIXED_MOTOR_SPEED);
        set_direction(1, 1);
        break;
    case mm_reverse:
        set_speed(FIXED_MOTOR_SPEED, FIXED_MOTOR_SPEED);
        set_direction(0, 0);
        break;
    case mm_center_left:
        set_speed(FIXED_MOTOR_SPEED, FIXED_MOTOR_SPEED);
        set_direction(1, 0);
        break;
    case mm_center_right:
        set_speed(FIXED_MOTOR_SPEED, FIXED_MOTOR_SPEED);
        set_direction(0, 1);
        break;
    case mm_left_forward_only:
        set_speed(FIXED_MOTOR_SPEED, 0);
        set_direction(1, 0);
        break;
    case mm_left_reverse_only:
        set_speed(FIXED_MOTOR_SPEED, 0);
        set_direction(0, 0);
        break;
    case mm_right_forward_only:
        set_speed(0, FIXED_MOTOR_SPEED);
        set_direction(0, 1);
        break;
    case mm_right_reverse_only:
        set_speed(0, FIXED_MOTOR_SPEED);
        set_direction(0, 0);
        break;
    
    default:
        shutdown(true);
        break;
    }

    last_random_move = millis();
}
#include <Arduino.h>
#include "robox_motor.h"
#include "general_definitions.h"

#define MAX_12_BITS 4095


RoboxMotor::RoboxMotor() {

}


void RoboxMotor::init() {

    // speed pins
    ledcAttachPin (MOTOR_1_SPD, 1); 
    ledcSetup(1, MOTOR_PWM_FREQ, MOTOR_TIMER_BITS);
    ledcAttachPin (MOTOR_2_SPD, 2);
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
    digitalWrite(MOTOR_1_DIR, (m1) ? HIGH : LOW);
    digitalWrite(MOTOR_2_DIR, (m2) ? HIGH : LOW);
}

void RoboxMotor::enable(bool enable) {
    // configure FAULT and ENABLE pin pullup
}

void RoboxMotor::shutdown(bool shutdown) {
    // configure RESET and SHUTDOWN pin
}

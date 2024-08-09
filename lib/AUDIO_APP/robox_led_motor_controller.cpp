#include "robox_led_motor_controller.h"
#if defined(ROBOX_LED_MOTOR_CONTROLLER)
// #include "freertos/task.h"

#include "robox_fft_beat.h"
#include "robox_motor.h"
#include "robox_restart.h"

// Statically allocate and initialize the spinlock
// static portMUX_TYPE robox_effects_logic = portMUX_INITIALIZER_UNLOCKED;

extern RoboxMotor* motor;
extern RoboxRestartManager restart_manager;

void LedMotorController::next() {

    // taskENTER_CRITICAL(&robox_effects_logic);
//  taskENTER_CRITICAL();
    switch (current)
    {
    case lm_none:
        current = lm_led;
        break;

    case lm_led:
        current = lm_motor;
        break;

    case lm_motor:
        current = lm_led_motor;
        break;
    
    case lm_led_motor:
    default:
        current = lm_none;
        break;
    }

    restart_manager.set_led_motor(current);

    if (is_motor_enabled()) {
        motor->enable(true);
    }
    else {
        motor->shutdown(true);
    }

    if (!is_led_enabled()) {
        led_clear();
    }
    // taskEXIT_CRITICAL(&robox_effects_logic);
//  taskEXIT_CRITICAL();
}

void LedMotorController::register_motor_shutdown(bool is_shutdown) {
    // taskENTER_CRITICAL(&robox_effects_logic);
//  taskENTER_CRITICAL();
    if (current == lm_motor) {
        current = lm_none;
    }
    else if (current == lm_led_motor) {
        current = lm_led;
    }
    // taskEXIT_CRITICAL(&robox_effects_logic);
//  taskEXIT_CRITICAL();
}

bool LedMotorController::is_led_enabled() {
    bool ret = false;

    // taskENTER_CRITICAL(&robox_effects_logic);
//  taskENTER_CRITICAL();
    if ((current == lm_led) || (current == lm_led_motor)) {
        ret = true;
    }
    // taskEXIT_CRITICAL(&robox_effects_logic);
//  taskEXIT_CRITICAL();

    return ret;
}

bool LedMotorController::is_motor_enabled() {
    bool ret = false;

    // taskENTER_CRITICAL(&robox_effects_logic);
//  taskENTER_CRITICAL();
    if ((current == lm_motor) || (current == lm_led_motor)) {
        ret = true;
    }
    // taskEXIT_CRITICAL(&robox_effects_logic);
//  taskEXIT_CRITICAL();

    return ret;
}

bool LedMotorController::is_nothing_enabled() {
    return (current == lm_none) ? true : false;
}

#endif
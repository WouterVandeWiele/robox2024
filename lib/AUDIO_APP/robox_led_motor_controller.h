#pragma once

#define ROBOX_LED_MOTOR_CONTROLLER

enum LedMotorEnum {lm_none, lm_led, lm_motor, lm_led_motor};

class LedMotorController {
    public:
        LedMotorController()
            : current(lm_none)
        {};

        void next();

        void register_motor_shutdown(bool is_shutdown);

        bool is_led_enabled();
        bool is_motor_enabled();
        bool is_nothing_enabled();
    
    private:
        LedMotorEnum current;
};
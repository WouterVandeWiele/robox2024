#pragma once

#define ROBOX_LED_MOTOR_CONTROLLER

enum LedMotorEnum {
    lm_none,        // led and motor not active
    lm_led,         // only led active
    lm_motor,       // only motor active
    lm_led_motor    // led and motor active
};

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
    
        void set_current(LedMotorEnum c) {current = c;};
    private:
        LedMotorEnum current;
};
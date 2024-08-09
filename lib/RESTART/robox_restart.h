#pragma once
#include <Arduino.h>
#include "robox_audio_mux.h"
#include "robox_led_motor_controller.h"

class RoboxRestartManager {
    public:
        RoboxRestartManager();

        uint32_t get_reboot_counter();

        void reboot_next_source(audio_source source);
        audio_source get_startup_source();

        void set_led_motor(LedMotorEnum lm);
        LedMotorEnum get_led_motor();

        bool is_cold_boot();
        void setupWifi();
        void resetWifiCred();
        String getDefaultName();
        void setupWifiOnDemand();

    private:
        bool cold_boot;


        void restart() {
            delay(100);
            esp_restart();
        };

        esp_reset_reason_t restartReason() {
            return esp_reset_reason();
        }
};

void wifi_suspend();
void wifi_start();
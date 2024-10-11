#pragma once
#include <Arduino.h>
#include "robox_audio_mux.h"
#include "robox_led_motor_controller.h"

class RoboxRestartManager {
    public:
        RoboxRestartManager();

        void init();

        uint32_t get_reboot_counter();

        void reboot_next_source(audio_source source);
        audio_source get_startup_source();
        uint8_t get_webplayer_startup_index();
        void set_webplayer_startup_index(uint8_t index);

        void set_led_motor(LedMotorEnum lm);
        LedMotorEnum get_led_motor();

        bool is_cold_boot();
        void setWifiSetupText();
        void setupWifi();
        void resetWifiCred();
        String getDefaultName();
        void setupWifiOnDemand();

        bool is_wifi_initialized();
        bool is_wifi_started();

    private:
        bool cold_boot;
        bool _is_wifi_initialized;
        bool _is_wifi_started;

        void restart() {
            delay(100);
            esp_restart();
        };

        esp_reset_reason_t restartReason() {
            return esp_reset_reason();
        }
};
#pragma once
#include <Arduino.h>
#include "robox_audio_mux.h"

// void restart_test();

class RoboxRestartManager {
    public:
        RoboxRestartManager();

        void reboot_next_source(audio_source source);
        uint32_t get_reboot_counter();
        audio_source get_startup_source();

        bool is_cold_boot();
        void setupWifi();

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
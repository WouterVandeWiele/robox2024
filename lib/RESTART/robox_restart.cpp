#include <Arduino.h>
#include "robox_restart.h"

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

static RTC_NOINIT_ATTR uint32_t _reboot_counter;
static RTC_NOINIT_ATTR uint8_t _audio_source;

static WiFiManager wifiManager;


RoboxRestartManager::RoboxRestartManager() {
    esp_reset_reason_t reason = restartReason();

    if ((reason != ESP_RST_DEEPSLEEP) && (reason != ESP_RST_SW)) {
        // invalidate variables
        _reboot_counter = 0;
        _audio_source = NotSelectedSource;
        cold_boot = true;
    }
    else {
        cold_boot = false;
    }
    
}

uint32_t RoboxRestartManager::get_reboot_counter() {
    return _reboot_counter;
}

bool RoboxRestartManager::is_cold_boot() {
    return cold_boot;
}

audio_source RoboxRestartManager::get_startup_source() {
    return (audio_source) _audio_source;
}

void RoboxRestartManager::reboot_next_source(audio_source source) {
    _audio_source = (uint8_t) source;
    restart();
}

void RoboxRestartManager::setupWifi() {
    if (_audio_source != BleSource) {
// #if defined(ROBOX_WIFI_MANAGER)
    // Run this part as soon as you need Wifi

        uint64_t _chipmacid = 0LL;
        esp_efuse_mac_get_default((uint8_t*) (&_chipmacid));
        String hostString = String((uint32_t)_chipmacid, HEX);
        hostString.toUpperCase();
        String ssid = "ROBOX_" + hostString;

        // use for testing, to clear the stored/last ssid/password
        // wifiManager.resetSettings();

        // automatically connect to wifi
        WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
        wifiManager.setDebugOutput(true, WM_DEBUG_MAX);
        // wifiManager.setConfigPortalBlocking(false);
        boolean result = wifiManager.autoConnect(ssid.c_str(), NULL); // empty password
        if (result)
        {
            Serial.println("Successfully connected to Wifi.");
            // server_setup();
            // server_start();
        }
        else
        {
            Serial.println("Failed setting up Wifi.");
        }
    }

// #endif
}


#include <Arduino.h>
#include "robox_restart.h"

#include "lcd_screen.h"
#include "robox_audio_mux.h"

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

static RTC_NOINIT_ATTR uint32_t _reboot_counter;
static RTC_NOINIT_ATTR uint8_t _audio_source;

static WiFiManager wifiManager;
extern RoboxAudioMux mux;

static void configModeCallback (WiFiManager *myWiFiManager) {
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());

    Serial.println(myWiFiManager->getConfigPortalSSID());

    String display_text = "Select Wifi Via   ->" + myWiFiManager->getConfigPortalSSID();

    std::lock_guard<std::mutex> lck(meta_data_mtx);
    mux.meta.title = String(display_text.c_str());
    lcd_invalidate(INVALIDATE_ALL);
}


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
    if (_audio_source == WebRadioSource) {
        setupWifiOnDemand();
    }
}

void RoboxRestartManager::setupWifiOnDemand() {
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
    wifiManager.setAPCallback(configModeCallback);
    // wifiManager.setDebugOutput(true, WM_DEBUG_MAX);
    // wifiManager.setConfigPortalBlocking(false);
    boolean result = wifiManager.autoConnect(ssid.c_str(), NULL); // empty password
    if (result)
    {
        Serial.println("Successfully connected to Wifi.");
        String display_text = "Wifi Connected    -> " + WiFi.localIP().toString() + String("                                        ");

        std::lock_guard<std::mutex> lck(meta_data_mtx);
        mux.meta.title = String(display_text.c_str());
        lcd_invalidate(INVALIDATE_ALL);
    }
    else
    {
        Serial.println("Failed setting up Wifi.");
        String display_text = "Wifi Failed";

        std::lock_guard<std::mutex> lck(meta_data_mtx);
        mux.meta.title = String(display_text.c_str()) + String("                                        ");
        lcd_invalidate(INVALIDATE_ALL);
    }
}


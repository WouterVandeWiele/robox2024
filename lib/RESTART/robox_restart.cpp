#include <Arduino.h>
#include "robox_restart.h"

#include "lcd_screen.h"
#include "robox_audio_mux.h"

#include "robox_language.h"

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

static RTC_NOINIT_ATTR uint32_t _reboot_counter;
static RTC_NOINIT_ATTR uint8_t _audio_source;
static RTC_NOINIT_ATTR uint8_t _led_motor;

static WiFiManager wifiManager;
extern RoboxAudioMux mux;

static void configModeCallback (WiFiManager *myWiFiManager) {
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());

    Serial.println(myWiFiManager->getConfigPortalSSID());

    String display_text = LANG_TOP_WIFI_SELECT + myWiFiManager->getConfigPortalSSID();

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
        _led_motor = lm_none;
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


void RoboxRestartManager::set_led_motor(LedMotorEnum lm) {
    _led_motor = (uint8_t) lm;
}

LedMotorEnum RoboxRestartManager::get_led_motor() {
    return (LedMotorEnum) _led_motor;
}



void RoboxRestartManager::setupWifi() {
    if (_audio_source == WebRadioSource) {
        setupWifiOnDemand();
    }
}

    void RoboxRestartManager::resetWifiCred() {
    wifiManager.resetSettings();
}

void RoboxRestartManager::setupWifiOnDemand() {
    // Run this part as soon as you need Wifi

    uint64_t _chipmacid = 0LL;
    esp_efuse_mac_get_default((uint8_t*) (&_chipmacid));
    String hostString = String((uint32_t)_chipmacid, HEX);
    hostString.toUpperCase();
    String ssid = "ROBOX_" + hostString;

    // automatically connect to wifi
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
    wifiManager.setAPCallback(configModeCallback);
    // wifiManager.setDebugOutput(true, WM_DEBUG_MAX);
    // wifiManager.setConfigPortalBlocking(false);
    boolean result = wifiManager.autoConnect(ssid.c_str(), NULL); // empty password
    if (result)
    {
        Serial.println("Successfully connected to Wifi.");
        String display_text = LANG_TOP_WIFI_CONNECTED + WiFi.localIP().toString() + String("                                        ");

        std::lock_guard<std::mutex> lck(meta_data_mtx);
        mux.meta.title = String(display_text.c_str());
        lcd_invalidate(INVALIDATE_ALL);
    }
    else
    {
        Serial.println("Failed setting up Wifi.");
        String display_text = LANG_TOP_WIFI_FAILED;

        std::lock_guard<std::mutex> lck(meta_data_mtx);
        mux.meta.title = String(display_text.c_str()) + String("                                        ");
        lcd_invalidate(INVALIDATE_ALL);
    }
}


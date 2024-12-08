#include <Arduino.h>
#include <Preferences.h>
#include "general_config.h"
#include "robox_restart.h"

#include "lcd_screen.h"
#include "robox_audio_mux.h"

#include "robox_language.h"

#include "robox_server.h"

#include "robox_fft_beat.h"

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include "updates.h"

static RTC_NOINIT_ATTR uint32_t _reboot_counter;
static RTC_NOINIT_ATTR uint8_t _audio_source;
static RTC_NOINIT_ATTR uint8_t _led_motor;
static RTC_NOINIT_ATTR uint8_t _web_index;


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

void RoboxRestartManager::setWifiSetupText() {
    String display_text = LANG_TOP_WIFI_SELECT + wifiManager.getConfigPortalSSID();

    std::lock_guard<std::mutex> lck(meta_data_mtx);
    mux.meta.title = String(display_text.c_str());
    lcd_invalidate(INVALIDATE_ALL);
}


RoboxRestartManager::RoboxRestartManager()
    : _is_wifi_initialized(false)
    , _is_wifi_started(false)
{}

void RoboxRestartManager::init() {
    Preferences roboxPrefs;
    esp_reset_reason_t reason = restartReason();

    // init NVS
    roboxPrefs.begin("roboxPrefs", RO_MODE); 
    
    bool nvsInit = roboxPrefs.isKey("nvsInit");       // Test for the existence
                                                      // of the "already initialized" key.

    if (nvsInit == false) {
      // If nvsInit is 'false', the key "nvsInit" does not yet exist therefore this
      //  must be our first-time run. We need to set up our Preferences namespace keys. So...
      roboxPrefs.end();                             // close the namespace in RO mode and...
      roboxPrefs.begin("roboxPrefs", RW_MODE);      //  reopen it in RW mode.

      roboxPrefs.putBool("nvsInit", true);      // Create the "already initialized"
                                                //  key and store a value.

      // The "factory defaults" are created and stored so...
    //   roboxPrefs.end();                             // Close the namespace in RW mode and...
    //   roboxPrefs.begin("roboxPrefs", RO_MODE);      //  reopen it in RO mode so the setup code
    //                                                 //  outside this first-time run 'if' block
    //                                                 //  can retrieve the run-time values
    //                                                 //  from the "roboxPrefs" namespace.
        _motor_speed_left = FIXED_MOTOR_SPEED;
        _motor_speed_right = FIXED_MOTOR_SPEED;
        _led_dimmer = MAX_LED_BRIGHTNESS;
    }
    else {
        _motor_speed_left = roboxPrefs.getFloat("motor_l", FIXED_MOTOR_SPEED);
        _motor_speed_right = roboxPrefs.getFloat("motor_r", FIXED_MOTOR_SPEED);
        _led_dimmer = roboxPrefs.getFloat("led_dim", MAX_LED_BRIGHTNESS);
    }

    roboxPrefs.end();

    set_brightness(_led_dimmer);

    // init variables
    if ((reason != ESP_RST_DEEPSLEEP) && (reason != ESP_RST_SW)) {
        // invalidate variables
        _reboot_counter = 0;
        _led_motor = lm_none;
        _web_index = 0;
        cold_boot = true;

        roboxPrefs.begin("roboxPrefs", RO_MODE);
        _audio_source = roboxPrefs.getUChar("audio_src", 0);
        roboxPrefs.end();

        Serial.println(_audio_source);

    }
    else {
        // hot boot, variables still initialized
        cold_boot = false;
    }
    
}

float RoboxRestartManager::get_motor_speed_left() {
    return _motor_speed_left;
}

float RoboxRestartManager::get_motor_speed_right() {
    return _motor_speed_right;
}

void RoboxRestartManager::set_motor_speed_left(float speed) {
    _motor_speed_left = speed;

    Preferences roboxPrefs;
    roboxPrefs.begin("roboxPrefs", RW_MODE);
    roboxPrefs.putFloat("motor_l", speed);
    roboxPrefs.end();

}
void RoboxRestartManager::set_motor_speed_right(float speed) {
    _motor_speed_right = speed;

    Preferences roboxPrefs;
    roboxPrefs.begin("roboxPrefs", RW_MODE);
    roboxPrefs.putFloat("motor_r", speed);
    roboxPrefs.end();
}

float RoboxRestartManager::get_led_dimmer() {
    return _led_dimmer;
}

void RoboxRestartManager::set_led_dimmer(float brightess) {
    _led_dimmer = brightess;

    Preferences roboxPrefs;
    roboxPrefs.begin("roboxPrefs", RW_MODE);
    roboxPrefs.putFloat("led_dim", brightess);
    roboxPrefs.end();

    set_brightness(_led_dimmer);
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

uint8_t RoboxRestartManager::get_webplayer_startup_index() {
    return _web_index;
}

void RoboxRestartManager::set_webplayer_startup_index(uint8_t index) {
    _web_index = index;
}


void RoboxRestartManager::reboot_next_source(audio_source source) {
    _audio_source = (uint8_t) source;

    Preferences roboxPrefs;
    roboxPrefs.begin("roboxPrefs", RW_MODE);
    roboxPrefs.putUChar("audio_src", _audio_source);
    roboxPrefs.end();

    Serial.println(_audio_source);

    restart();
}


void RoboxRestartManager::set_led_motor(LedMotorEnum lm) {
    _led_motor = (uint8_t) lm;
}

LedMotorEnum RoboxRestartManager::get_led_motor() {
    return (LedMotorEnum) _led_motor;
}



void RoboxRestartManager::setupWifi() {
    if ((_audio_source == WebRadioSource) || (_audio_source == NotSelectedSource)) {
        setupWifiOnDemand();
    }

    if (_audio_source == NotSelectedSource) {
        server_setup();
        server_start();
    }
}

void RoboxRestartManager::resetWifiCred() {
    wifiManager.resetSettings();
}

String RoboxRestartManager::getDefaultName() {
    uint64_t _chipmacid = 0LL;
    esp_efuse_mac_get_default((uint8_t*) (&_chipmacid));
    String hostString = String((uint32_t)_chipmacid, HEX);
    hostString.toUpperCase();
    String ssid = "ROBOX_" + hostString;

    return ssid;
}

void RoboxRestartManager::setupWifiOnDemand() {
    // Run this part as soon as you need Wifi
    _is_wifi_initialized = false;
    _is_wifi_started = true;

    String ssid = getDefaultName();

    std::vector<const char*> wmMenuItems = { "wifi", "info", "exit" };

    // automatically connect to wifi
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
    wifiManager.setMenu(wmMenuItems);
    wifiManager.setHostname(ssid);
    wifiManager.setAPCallback(configModeCallback);

    #if defined(DEBUG_RESET_WIFI_CREDENTIALS_AT_START)
    wifiManager.resetSettings();
    #endif
    // wifiManager.setConnectTimeout(20);
    // wifiManager.setConnectRetries(100);
    // wifiManager.setDebugOutput(true, WM_DEBUG_MAX);
    // wifiManager.setConfigPortalBlocking(false);
    boolean result = wifiManager.autoConnect(ssid.c_str(), NULL); // empty password
    if (result)
    {
        _is_wifi_initialized = true;
        Serial.println("Successfully connected to Wifi.");
        String display_text = LANG_TOP_WIFI_CONNECTED + WiFi.localIP().toString() + String("                                        ");

        #if defined(UPDATE_CHECK)
        update_check();
        #endif

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

bool RoboxRestartManager::is_wifi_initialized() {
    return _is_wifi_initialized;
}

bool RoboxRestartManager::is_wifi_started() {
    return _is_wifi_started;
}
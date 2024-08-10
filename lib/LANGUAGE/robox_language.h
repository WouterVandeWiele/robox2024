#pragma once

// #include <Arduino.h>
// #include <map>

// enum L_Language {lang_en, lang_nl};
// enum L_Messgaes {lang_no_source, lang_switch_ble, lang_switch_web, lang_switch_sd, lang_menu_return};

// typedef const std::map<L_Messgaes, const String> vTransMap;
// typedef const String _cStr;

// extern vTransMap l_en;

// class Translator {
//     public:
//         Translator(L_Language lang)
//         {};

//         const String get_text(L_Messgaes message);
//         void switch_language(L_Language lang);

//     private:
//         const vTransMap* current_lang_map;
// };

#include "general_config.h"

#if defined(LANG_EN)

#define LANG_SOURCE_NO "No Source"
#define LANG_SOURCE_BLE "Bluetooth"
#define LANG_SOURCE_WEB "Web Radio"
#define LANG_SOURCE_SD "MP3-Player"

#define LANG_PRINT "Print"
#define LANG_BACK "Back"

#define LANG_RESET_WIFI_CRED "Reset Wifi Cred"

#define LANG_MENU_SWITCH "Switch Audio Source"
#define LANG_MENU_SETTINGS "Settings"

#define LANG_TOP_SELECT_SOURCE "<- Select Source"
#define LANG_TOP_BLE_NAME "BLE Name          -> "
#define LANG_TOP_WIFI_SELECT "Select Wifi Via   ->"
#define LANG_TOP_WIFI_CONNECTED "Wifi Connected    -> "
#define LANG_TOP_WIFI_FAILED "Wifi Setup Failed"

#define LANG_RO_WIFI_SSID "Wifi Name"
#define LANG_RO_BLE_NAME "Bluetooth Name"
#define LANG_RO_IP_ADDRESS "IP Address"
#define LANG_RO_BATTERY_VOLTAGE "Battery Voltage"

#define LANG_RO_RO "~~Read Only~~~"

#endif

#if defined(LANG_NL)

#define LANG_SOURCE_NO "Geen Audio"
#define LANG_SOURCE_BLE "Bluetooth"
#define LANG_SOURCE_WEB "Internetradio"
#define LANG_SOURCE_SD "MP3-Speler"

#define LANG_PRINT "Print"
#define LANG_BACK "Terug"

#define LANG_RESET_WIFI_CRED "Reset Wifi Cred"

#define LANG_MENU_SWITCH "Wissel Audio Bron"
#define LANG_MENU_SETTINGS "Instellingen"

#define LANG_TOP_SELECT_SOURCE "<- Wissel Bron"
#define LANG_TOP_BLE_NAME "BLE Naam          -> "
#define LANG_TOP_WIFI_SELECT "Selecteer Wifi Via->"
#define LANG_TOP_WIFI_CONNECTED "Wifi Geconnecteerd-> "
#define LANG_TOP_WIFI_FAILED    "Wifi Niet Verbonden"

#define LANG_RO_WIFI_SSID "Wifi Naam"
#define LANG_RO_BLE_NAME "Bluetooth Naam"
#define LANG_RO_IP_ADDRESS "IP Adres"
#define LANG_RO_BATTERY_VOLTAGE "Batterij Spanning"

#define LANG_RO_RO "~~Enkel Lezen~"

#endif
// https://github.com/witnessmenow/ESP-Web-Tools-Tutorial/blob/main/CodeExamples/BlinkWhenConnected/BlinkWhenConnected.ino

#include <Arduino.h>
#include "WiFi.h"
#include "robox_fft_beat.h"

//This is needed for some of the Wifi functions we are using
#include <esp_wifi.h>

static bool connectWifi(String ssid, String pass) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  int retry = 0;
  while (retry < 10 && WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    retry++;
    delay(500);
  }

  return WiFi.status() == WL_CONNECTED;
}

static bool autoConnectToWifi() {
  String ssid;
  String pass;

  wifi_config_t conf;
  esp_wifi_get_config(WIFI_IF_STA, &conf);

  ssid = String(reinterpret_cast<const char*>(conf.sta.ssid));
  pass = String(reinterpret_cast<char*>(conf.sta.password));

  Serial.println("autoConnectToWifi");
  Serial.println(ssid);
  Serial.println(pass);

  bool connected = false;
  // Check if the ESP auto connected
  if (WiFi.status() == WL_CONNECTED) {
    connected = true;
  }

  // If it didn't auto connect, try connect with the stored
  // details
  if (connected || connectWifi(ssid, pass)) {

    return true; // connected success
  }

  return false;
}

void wifi_setup() {
    //AutoConnect will try connect to Wifi using details stored in flash
  if (!autoConnectToWifi()) {
    bool exitWifiConnect = false;
    String ssid = "";
    String pass = "";
    int stage = 0;
    Serial.setTimeout(20000); //20 seconds
    //Wait here til we get Wifi details
    while (!exitWifiConnect) {

      Serial.println("Please enter your Wifi SSID:");
      ssid = Serial.readStringUntil('\r'); // Web Flash tools use '\r\n' for line endings
      Serial.read();
      if (ssid != "") {
        Serial.println("Please enter your Wifi password:");
        pass = Serial.readStringUntil('\r');
        Serial.read();

        exitWifiConnect = connectWifi(ssid, pass);
      }
      if (!exitWifiConnect) {
        Serial.println("Failed to connect:");
      } else {
        Serial.println("connect");
      }
    }
  }

  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}
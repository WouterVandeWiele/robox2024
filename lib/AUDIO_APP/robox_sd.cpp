#include "robox_i2s.h"
#include "general_definitions.h"

#include "robox_sd.h"
#include "general_definitions.h"

#define USE_HELIX 
#define USE_SDFAT



static void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}


void RoboxSD::mux_start() {
    ESP_LOGI(LOG_SD_TAG, ">>> SD starting...");

    i2s_setup();

    // setup player
    player.setMetadataCallback(printMetaData);
    player.begin();

    player.setVolume(0.2);

    ESP_LOGI(LOG_BLE_TAG, "<<< SD setup completed");
}

void RoboxSD::mux_stop() {
  player.end();
}

void RoboxSD::mux_copy() {
  player.copy();
}
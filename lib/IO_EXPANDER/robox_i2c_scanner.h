#ifndef ROBOX_I2C_SCANNER
#define ROBOX_I2C_SCANNER

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include "esp_log.h"
#include "general_definitions.h"

void scanner_setup()
{
    Wire.begin(I2C_SDA, I2C_SCL, I2C_SPEED);
}

void scanner_loop() {
    ESP_LOGI(LOG_I2C_SCAN_TAG, "I2C scanner. Scanning ...");
    uint16_t count = 0;

    for (uint16_t i = 0; i <= 255; i++)
    {
        Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
        int res = Wire.endTransmission ();
        // Serial.println(res);
        if (res == 0)  // Receive 0 = success (ACK response) 
        {
        Serial.printf("Found address: %2d (0x%2x)\n", i, i);
        count++;
        }
    }
    Serial.printf("Found %d device(s).\n", count);
}

#endif  // ROBOX_I2C_SCANNER
#ifndef ROBOX_I2C_SCANNER
#define ROBOX_I2C_SCANNER

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
    uint8_t count = 0;

    for (uint8_t i = 0; i <= 255; i++)
    {
        Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
        int res = Wire.endTransmission ();
        // Serial.println(res);
        if (res == 0)  // Receive 0 = success (ACK response) 
        {
        ESP_LOGI(LOG_I2C_SCAN_TAG, "Found address: %2d (0x%2x)", i, i);
        count++;
        }
    }
    ESP_LOGI(LOG_I2C_SCAN_TAG, "Found %d device(s).", count);
}

#endif  // ROBOX_I2C_SCANNER
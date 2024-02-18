#include <Arduino.h>
// #include "lcd_config.h"
#include "debug_cli.h"

void setup() {
    Serial.begin(115200);
    // lcd_setup();
    debug_cli_setup();
}

void loop() {
    // lcd_test();
    debug_cli_loop();
}
#include <Arduino.h>
#include "lcd_config.h"
// #include "debug_cli.h"
// #include "audio.h"

void setup() {
    Serial.begin(115200);
    lcd_setup();
    // debug_cli_setup();
    // audio_setup();
}

void loop() {
    lcd_test();
    // debug_cli_loop();
    // delay(100);
}
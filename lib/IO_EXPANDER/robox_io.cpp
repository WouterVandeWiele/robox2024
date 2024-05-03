#include "robox_io.h"
#include "Arduino.h"
#include <Wire.h> // TwoWire
// #include "esp_log.h"


#define PORT_ADDRESS(port, low, high) (port ? high : low)

RoboxIoExpander::RoboxIoExpander(uint8_t io_address): io_address(io_address) {
    pinMode(IO_RESET, OUTPUT);
    io_reset();

    Wire.begin();
    Wire.setClock(400000L);
}

RoboxIoExpander::~RoboxIoExpander() {
    Wire.end();
}


void RoboxIoExpander::io_set(uint8_t reg_address, uint8_t data) {
    Wire.beginTransmission(io_address);
    Wire.write(reg_address);
    Wire.write(data);
    Wire.endTransmission();
}

uint8_t RoboxIoExpander::io_get(uint8_t reg_address) {
    Wire.beginTransmission(io_address);
    Wire.write(reg_address);
    Wire.endTransmission();


    Wire.requestFrom(io_address, 0x1);
    uint8_t data = Wire.read();

    return data;
}

void RoboxIoExpander::io_reset() {
    digitalWrite(IO_RESET, HIGH);
    delay(1);
    digitalWrite(IO_RESET, LOW);
    delay(1);
    digitalWrite(IO_RESET, HIGH);
}



uint8_t RoboxIoExpander::get_inputs(uint8_t port){
    return io_get(PORT_ADDRESS(port, INPUT_P0, INPUT_P1));
}

void RoboxIoExpander::configure_outputs(uint8_t port, uint8_t data) {
    ESP_LOGI(LOG_I2C_TAG, "IO expander configure output port %d - %2x", port, data);
    io_set(PORT_ADDRESS(port, CONFIG_P0, CONFIG_P1), data);
}

uint8_t RoboxIoExpander::get_configure_outputs(uint8_t port) {
    return io_get(PORT_ADDRESS(port, CONFIG_P0, CONFIG_P1));
}

void RoboxIoExpander::set_output(uint8_t port, uint8_t data) {
    io_set(PORT_ADDRESS(port, OUTPUT_P0, OUTPUT_P1), data);
}


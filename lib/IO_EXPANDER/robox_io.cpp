#include <Arduino.h>
#include <mutex>
#include <Wire.h> // TwoWire
#include "robox_io.h"
// #include "esp_log.h"
#include "general_definitions.h"

#define PORT_ADDRESS(port, low, high) (port ? high : low)

static bool io_interrupt_generated = false;
std::mutex io_operations;

void IRAM_ATTR io_isr() {
	io_interrupt_generated = true;
}


RoboxIoExpander::RoboxIoExpander(uint8_t io_address)
    : io_address(io_address)
    , config()
    , observers()
    , _out_buffer()
    // , interrupt_data({.data={0x00, 0x00}, .flag=false})
{
    pinMode(IO_RESET, OUTPUT);
    // pinMode(IO_INTERRUPT, INPUT);

    io_reset();

    // attachInterrupt(IO_INTERRUPT, io_isr, FALLING);

    // for (IoInterface* o: observers) {
    //     config.push_back(o->io_config());
    // }

    Wire.begin(I2C_SDA, I2C_SCL, I2C_SPEED);
}


RoboxIoExpander::~RoboxIoExpander() {
    Wire.end();
}

void RoboxIoExpander::register_observer(IoInterface* observer, ExpanderConfig observer_config) {
    observers.push_back(observer);
    config.push_back(observer_config);
}


void RoboxIoExpander::io_set(uint8_t reg_address, uint8_t data) {
    Wire.beginTransmission(io_address);
    Wire.write(reg_address);
    Wire.write(data);
    Wire.endTransmission();
}

void RoboxIoExpander::io_set_multi(uint8_t reg_address, std::vector<uint8_t> data) {
    Wire.beginTransmission(io_address);
    Wire.write(reg_address);
    for (uint8_t d: data) {
        Wire.write(d);
    }
    Wire.endTransmission();
}

uint8_t RoboxIoExpander::io_get(uint8_t reg_address) {
    Wire.beginTransmission(io_address);
    Wire.write(reg_address);
    Wire.endTransmission(false);


    Wire.requestFrom(io_address, 1);
    uint8_t data = Wire.read();
    Wire.endTransmission(true);

    return data;
}

std::vector<uint8_t> RoboxIoExpander::io_get_multi(uint8_t reg_address, uint8_t num_bytes) {
    std::vector<uint8_t> data;

    Wire.beginTransmission(io_address);
    Wire.write(reg_address);
    Wire.endTransmission();


    Wire.requestFrom(io_address, num_bytes);
    for (uint8_t i = 0; i <num_bytes; i++) {
        data.push_back(Wire.read());
    }

    return data;
}

void RoboxIoExpander::io_reset() {
    digitalWrite(IO_RESET, HIGH);
    delay(10);
    digitalWrite(IO_RESET, LOW);
    delay(10);
    digitalWrite(IO_RESET, HIGH);
}



uint8_t RoboxIoExpander::get_inputs(uint8_t port) {
    // only used by battery and motor fault
    std::lock_guard<std::mutex> lck(io_operations);
    return io_get(PORT_ADDRESS(port, INPUT_P0, INPUT_P1));
}

// void RoboxIoExpander::configure_outputs(uint8_t port, uint8_t data) {
//     ESP_LOGI(LOG_I2C_TAG, "IO expander configure output port %d - %2x", port, data);
//     io_set(PORT_ADDRESS(port, CONFIG_P0, CONFIG_P1), data);
// }

// uint8_t RoboxIoExpander::get_configure_outputs(uint8_t port) {
//     return io_get(PORT_ADDRESS(port, CONFIG_P0, CONFIG_P1));
// }

void RoboxIoExpander::set_output(uint8_t port, uint8_t data, uint8_t mask) {
    // const std::lock_guard<std::mutex> lock(io_mutex);

    // Serial.printf("IO expander set output port %d - %2x\n", port, data);

    _out_buffer[port] = (data & mask) | (_out_buffer[port] & ~(mask));

    // Serial.printf("IO expander write: %d\n", _out_buffer[port]);
    
    io_set(PORT_ADDRESS(port, OUTPUT_P0, OUTPUT_P1), _out_buffer[port]);

    // Serial.println("data written");
    // io_set(PORT_ADDRESS(port, OUTPUT_P0, OUTPUT_P1), data);
}

ExpanderConfig RoboxIoExpander::get_default_config() {
    ExpanderConfig config = {
        .out_state = {0xFF, 0xFF},
        .polarity = {0x00, 0x00},
        .out_config = {0xFF, 0xFF},
        .drive_strength = {0xFF, 0xFF, 0xFF, 0xFF},
        .input_latch = {0x00, 0x00},
        .pull_en = {0x00, 0x00},
        .pull_select = {0xFF, 0xFF},
        .interrupts = {0xFF, 0xFF},
    }; 
    return config;
}

void RoboxIoExpander::io_configure() {
    ExpanderConfig agg = get_default_config();

    for (ExpanderConfig c: config) {
        for (int8_t i = 0; i < 2; i++){
            agg.out_state[i] &= c.out_state[i];
            agg.polarity[i] |= c.polarity[i];
            agg.out_config[i] &= c.out_config[i];
            agg.drive_strength[i] &= c.drive_strength[i];
            agg.input_latch[i] |= c.input_latch[i];
            agg.pull_en[i] |= c.pull_en[i];
            agg.pull_select[i] &= c.pull_select[i];
            agg.interrupts[i] |= c.interrupts[i];
        }
        // drive strength
        agg.drive_strength[2] &= c.drive_strength[2];
        agg.drive_strength[3] &= c.drive_strength[3];
    }

    ESP_LOGI(LOG_I2C_TAG, "I2C config:");
    ESP_LOGI(LOG_I2C_TAG, "- out_state:      %2x - %2x", agg.out_state[0], agg.out_state[1]);
    ESP_LOGI(LOG_I2C_TAG, "- polarity:       %2x - %2x", agg.polarity[0], agg.polarity[1]);
    ESP_LOGI(LOG_I2C_TAG, "- out_config:     %2x - %2x", agg.out_config[0], agg.out_config[1]);
    ESP_LOGI(LOG_I2C_TAG, "- drive_strength: %2x %2x - %2x %2x", agg.drive_strength[0], agg.drive_strength[1], agg.drive_strength[2], agg.drive_strength[3]);
    ESP_LOGI(LOG_I2C_TAG, "- input_latch:    %2x - %2x", agg.input_latch[0], agg.input_latch[1]);
    ESP_LOGI(LOG_I2C_TAG, "- pull_en:        %2x - %2x", agg.pull_en[0], agg.pull_en[1]);
    ESP_LOGI(LOG_I2C_TAG, "- pull_select:    %2x - %2x", agg.pull_select[0], agg.pull_select[1]);
    ESP_LOGI(LOG_I2C_TAG, "- interrupts:     %2x - %2x", agg.interrupts[0], agg.interrupts[1]);

    // _out_buffer[0] = agg.out_state[0];
    // _out_buffer[1] = agg.out_state[1];

    io_set(OUTPUT_P0, agg.out_state[0]);
    io_set(OUTPUT_P1, agg.out_state[1]);
    io_set(POL_INV_P0, agg.polarity[0]);
    io_set(POL_INV_P1, agg.polarity[1]);
    io_set(CONFIG_P0, agg.out_config[0]);
    io_set(CONFIG_P1, agg.out_config[1]);

    io_set(OUTPUT_DRIVE_0_P0, agg.drive_strength[0]);
    io_set(OUTPUT_DRIVE_1_P0, agg.drive_strength[1]);
    io_set(OUTPUT_DRIVE_0_P1, agg.drive_strength[2]);
    io_set(OUTPUT_DRIVE_1_P1, agg.drive_strength[3]);
    io_set(INPUT_LATCH_P0, agg.input_latch[0]);
    io_set(INPUT_LATCH_P1, agg.input_latch[1]);
    io_set(PUPD_ENABLE_P0, agg.pull_en[0]);
    io_set(PUPD_ENABLE_P1, agg.pull_en[1]);
    io_set(PUPD_SELECT_P0, agg.pull_select[0]);
    io_set(PUPD_SELECT_P1, agg.pull_select[1]);
    io_set(INT_MASK_P0, agg.interrupts[0]);
    io_set(INT_MASK_P1, agg.interrupts[1]);

    Serial.printf("input0 %d\n", get_inputs(0));
    Serial.printf("input1 %d\n", get_inputs(1));
}

void RoboxIoExpander::loop() {
    if (io_interrupt_generated) {
        // an interrupt was generated
        std::vector<uint8_t> interrupt = io_get_multi(INT_STATUS_P0, 2);
        std::vector<uint8_t> data = io_get_multi(INPUT_P0, 2);

        auto itO = observers.begin();
        auto itC = config.begin();

        while(itO != observers.end() || itC != config.end()) {
            ++itO;
            ++itC;

            if (((*itC).interrupts[0] && interrupt[0]) || ((*itC).interrupts[1] && interrupt[1])) {
                (*itO)->io_interrupt_observer(data);
            }
        }

        io_interrupt_generated = false;
    }
}

#include "robox_io.h"
#include "Arduino.h"
// #include "Wire.h"
// #include <SoftwareWire.h>
// #include "PI4IOE5V6416.h"
// #include <PI4IOE5V6416.h>
// #include "driver/i2c.h"
// #include "esp_log.h"
#include <Wire.h> // TwoWire
// #include <AceWire.h>

// PI4IOE5V6416 ioex;

#define PORT_ADDRESS(port, low, high) (port ? high : low)
// static uint32_t i2c_frequency = I2C_SPEED;

// i2c_config_t conf = {
//     .mode = I2C_MODE_MASTER,
//     .sda_io_num = I2C_SDA,
//     .scl_io_num = I2C_SCL,
//     .sda_pullup_en = GPIO_PULLUP_ENABLE,
//     .scl_pullup_en = GPIO_PULLUP_ENABLE,
//     .master = {.clk_speed = I2C_SPEED},
//     .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
// };

RoboxIoExpander::RoboxIoExpander(uint8_t io_address): io_address(io_address) {
    // i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    // i2c_master_driver_initialize();
    // Wire.begin();
    // ioex.attach(Wire);
    // ioex.polarity(PI4IOE5V64XX::Polarity::ORIGINAL_ALL);
    // ioex.direction(PI4IOE5V64XX::Direction::OUT_ALL);
    // ioex.write(PI4IOE5V64XX::Level::L_ALL);


    pinMode(IO_RESET, OUTPUT);
    io_reset();

    Wire.begin();
    Wire.setClock(400000L);
}

RoboxIoExpander::~RoboxIoExpander() {
    // i2c_driver_delete(I2C_PORT);
    Wire.end();
}

void RoboxIoExpander::i2c_master_driver_initialize(void)
{
    

    // ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &conf));
}





void RoboxIoExpander::io_set(uint8_t reg_address, uint8_t data) {

    // myWire.beginTransmission(IO_EXPANDER_W_ADDRESS);
    // myWire.write(reg_address);
    // myWire.write(data);
    // Wire.beginTransmission(IO_EXPANDER_W_ADDRESS);
    // Wire.printf("%d%d", reg_address, data);
    // Wire.write(reg_address);
    // Wire.write(data);

    // ESP_LOGI(LOG_I2C_TAG, "End transmission");
    // uint8_t error = Wire.endTransmission(true);
    // // uint8_t error = myWire.endTransmission();
    // ESP_LOGI(LOG_I2C_TAG, "endTransmission: %u\n", error);


    // i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    // i2c_master_driver_initialize();

    // i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // i2c_master_start(cmd);
    // i2c_master_write_byte(cmd, io_address << 1 | WRITE_BIT, 0);
    // // if (1) {
    //     delayMicroseconds(10);
    //     i2c_master_write_byte(cmd, reg_address, 0);
    // // }
    // // for (int i = 0; i < 1; i++) {
    //     delayMicroseconds(10);
    //     i2c_master_write_byte(cmd, data, 0);
    // // }
    // delayMicroseconds(10);
    // i2c_master_stop(cmd);
    // esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    // i2c_cmd_link_delete(cmd);
    // if (ret == ESP_OK) {
    //     ESP_LOGI(LOG_I2C_TAG, "Write OK");
    // } else if (ret == ESP_ERR_TIMEOUT) {
    //     ESP_LOGW(LOG_I2C_TAG, "Bus is busy");
    // } else {
    //     ESP_LOGW(LOG_I2C_TAG, "Write Failed");
    // }
    // i2c_driver_delete(I2C_PORT);

    // using WireInterface = ace_wire::SimpleWireInterface;
    // WireInterface wireInterface(21, 22, 5);

    // Wire.begin();
    // Wire.setClock(400000L);
    // Wire.begin();
    // runBenchmark(F("TwoWireInterface<TwoWire>,400kHz"), wireInterface);

    // Send I2C address: 1 byte
    Wire.beginTransmission(0x20);
    // if (res) SERIAL_PORT_MONITOR.println(F("Error: beginTransmission()"));
    // if (res) ESP_LOGW(LOG_I2C_TAG, "Error: beginTransmission() %d", res);

    // Send start at position 7: 1 byte
    Wire.write(reg_address);
    // res = wireInterface.write(0x07);
    // if (res == 0) SERIAL_PORT_MONITOR.println(F("Error: write(command)"));
    // if (res) ESP_LOGW(LOG_I2C_TAG, "Error: write(command) %d", res);

    // Send 7 bytes to the alarm1 and alarm2 registers. Using the alarm registers
    // avoids changing the date and time stored on the DS3231 in case it is
    // actually being used as an active date/time source.
    Wire.write(data); // alarm1 seconds
    // Wire.write(data); // alarm1 seconds
    // res = wireInterface.write(0x1f); // alarm1 seconds
    // res = wireInterface.write(0x02); // alarm1 minutes
    // res = wireInterface.write(0x03); // alarm1 hours
    // res = wireInterface.write(0x04); // alarm1 day/date
    // res = wireInterface.write(0x05); // alarm2 minutes
    // res = wireInterface.write(0x06); // alarm2 hours
    // res = wireInterface.write(0x07); // alarm2 day/date

    // For buffered implementations, this causes the actual bit transfers. For
    // unbuffered implementations, this causes just the STOP condition to be sent.
    // Total transfer: 7 bytes
    Wire.endTransmission();
    // if (res) SERIAL_PORT_MONITOR.println(F("Error: endTransmission()"));
    // if (res) ESP_LOGW(LOG_I2C_TAG, "Error: endTransmission() %d", res);
    // wireInterface.end();
    // #if ! defined(ESP8266)
    // Wire.end();
    // delay(100);
    // #endif
}

uint8_t RoboxIoExpander::io_get(uint8_t reg_address) {
    // myWire.beginTransmission(IO_EXPANDER_W_ADDRESS);
    // myWire.write(reg_address);
    // Wire.beginTransmission(IO_EXPANDER_R_ADDRESS);
    // Wire.write(reg_address);

    // uint8_t received = Wire.requestFrom((io_address | 0x01), 1, true);

    // return received;
    // return 0;


    // uint8_t data;

    // i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    // i2c_master_start(cmd);
    // // if (data_addr != -1) {
    // i2c_master_write_byte(cmd, io_address << 1 | WRITE_BIT, ACK_CHECK_EN);
    // i2c_master_write_byte(cmd, reg_address, ACK_CHECK_EN);
    // i2c_master_start(cmd);
    // // }
    // i2c_master_write_byte(cmd, io_address << 1 | READ_BIT, ACK_CHECK_EN);
    // // if (len > 1) {
    // i2c_master_read(cmd, &data, 1 - 1, ACK_VAL);
    // // }
    // i2c_master_read_byte(cmd, &data, NACK_VAL);
    // i2c_master_stop(cmd);
    // esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    // i2c_cmd_link_delete(cmd);
    // if (ret == ESP_OK) {
    //     // for (int i = 0; i < len; i++) {
    //     ESP_LOGI(LOG_I2C_TAG, "0x%02x ", data);
    //     // if ((i + 1) % 16 == 0) {
    //     //     printf("\r\n");
    //     // }
    //     // }
    //     // if (len % 16) {
    //     //     printf("\r\n");
    //     // }
    // } else if (ret == ESP_ERR_TIMEOUT) {
    //     ESP_LOGW(LOG_I2C_TAG, "Bus is busy");
    // } else {
    //     ESP_LOGW(LOG_I2C_TAG, "Read failed");
    // }

    // return data;
    return 0;
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
    // ESP_LOGI(LOG_I2C_TAG, "IO expander set output port %d - %2x", port, data);
    io_set(PORT_ADDRESS(port, OUTPUT_P0, OUTPUT_P1), data);
    // if (data) {
    //     ioex.write(PI4IOE5V64XX::Level::H_ALL);
    // }
    // else {
    //     ioex.write(PI4IOE5V64XX::Level::L_ALL);
    // }
}


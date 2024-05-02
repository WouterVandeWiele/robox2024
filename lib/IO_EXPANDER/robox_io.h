#ifndef IOEXPANDER_H
#define IOEXPANDER_H
#include <stdint.h>
// #include "Wire.h"
// #include <SoftwareWire.h>
// #include "PI4IOE5V6416.h"
#include "general_definitions.h"

#define INPUT_P0 0x00   // input (incomming logic levels) port 0
#define INPUT_P1 0x01   // input (incomming logic levels) port 1

#define OUTPUT_P0 0x02  // output (outgoing logic levels) port 0
#define OUTPUT_P1 0x03  // output (outgoing logic levels) port 1

#define POL_INV_P0 0x04 // polarity inversion port 0
#define POL_INV_P1 0x05 // polarity inversion port 1

#define CONFIG_P0 0x06  // configuration (I/O direction) port 0
#define CONFIG_P1 0x07  // configuration (I/O direction) port 1

#define OUTPUT_DRIVE_0_P0 0x40  // output drive strength 0-3 port 0
#define OUTPUT_DRIVE_1_P0 0x41  // output drive strength 4-7 port 0
#define OUTPUT_DRIVE_0_P1 0x42  // output drive strength 0-3 port 1
#define OUTPUT_DRIVE_1_P1 0x43  // output drive strength 4-7 port 1

#define INPUT_LATCH_P0 0x44 // input (interrupt) latch register port 0
#define INPUT_LATCH_P1 0x45 // input (interrupt) latch register port 1

#define PUPD_ENABLE_P0 0x46 // pull-up/pull-down enable port 0
#define PUPD_ENABLE_P1 0x47 // pull-up/pull-down enable port 1

#define PUPD_SELECT_P0 0x48 // pull-up/pull-down selection port 0
#define PUPD_SELECT_P1 0x49 // pull-up/pull-down selection port 1

#define INT_MASK_P0 0x4A    // interrupt mask port 0
#define INT_MASK_P1 0x4B    // interrupt mask port 1

#define INT_STATUS_P0 0x4C  // interrupt status port 0
#define INT_STATUS_P1 0x4D  // interrupt status port 1

#define OUTPUT_CONFIG 0x4F

// PI4IOE5V6416 ioex;

class RoboxIoExpander {
    public:
        RoboxIoExpander(uint8_t address); //: io_address(address), reset_pin(reset) {
            // pinMode(reset, OUTPUT);

            // io_reset();

            // ESP_LOGI(LOG_I2C_TAG, "IO expander init");
            // myWire.setClock(I2C_SPEED);
            // myWire.begin();
            // Wire.begin(I2C_SDA, I2C_SCL, I2C_SPEED);

            // default, SDA: 21, SCL: 22, freq: 100kHz
            // Wire.begin();
            // Wire.setClock(I2C_SPEED);
            // Wire.setBufferSize(100);



            // ioex.attach(Wire);
            // ioex.polarity(PI4IOE5V64XX::Polarity::ORIGINAL_ALL);
            // ioex.direction(PI4IOE5V64XX::Direction::OUT_ALL);
            // ioex.write(PI4IOE5V64XX::Level::L_ALL);
            // Wire.setTimeOut(0xFFFFF);
        //};
        ~RoboxIoExpander();

        uint8_t get_inputs(uint8_t port);
        void configure_outputs(uint8_t port, uint8_t data);
        uint8_t get_configure_outputs(uint8_t port);
        void set_output(uint8_t port, uint8_t data);



    // RoboxIoExpander::~RoboxIoExpander()
    private:

        void i2c_master_driver_initialize(void);
        uint8_t io_address;
        // uint8_t reset_pin;
        // SoftwareWire myWire;

        void io_set(uint8_t address, uint8_t data);
        uint8_t io_get(uint8_t address);
        void io_reset();
        
};


#endif  // IOEXPANDER_H
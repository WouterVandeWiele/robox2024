#ifndef IOEXPANDER_H
#define IOEXPANDER_H

#include <mutex>
#include <vector>
#include <stdint.h>
#include "general_definitions.h"
// #include "lcd_screen.h"
// #include "robox_mux_interface.h"


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


#define CONFIG_START_ADDRESS1 0x02
#define CONFIG_START_ADDRESS2 0x40


// struct InterruptData {
// 	uint8_t data[2];
// 	bool flag;
// };

extern std::mutex io_operations;


typedef struct ExpanderConfig{
  uint8_t out_state[2];         // default ALL 1 - if output: 1 = write HIGH, 0 = write LOW
  uint8_t polarity[2];          // dedault ALL 0 - non inverted output
  uint8_t out_config[2];        // default ALL 1 - 1 = input pin, 0 = output pin
  uint8_t drive_strength[4];    // default ALL 1 - output drive level: 0b11 = 10mA, 0b10 = 7,5mA, 0b01 = 5mA, 0b00 = 2,5mA
  uint8_t input_latch[2];       // default ALL 0 - if latched (1) interupt only cleard on a read and the inputs state is kept untill a read
  uint8_t pull_en[2];           // default ALL 0 - no pull-up or pull-down enabled
  uint8_t pull_select[2];       // default ALL 1 - 1 = pull-up, 0 = pull-down
  uint8_t interrupts[2];        // default ALL 1 - 1 = no interrupts, set to 0 to generate interrupts on input pin
//   void (*int_observer)(uint8_t *data, void *this_pointer);

} ExpanderConfig;


 
class IoInterface {
    public:

        // static method that returns the IO expander config expected by this component
        // ExpanderConfig io_get_config() {
        //     return child_type::io_config();
        // }
        // static ExpanderConfig io_config() {

        // }

        virtual void io_interrupt_observer(std::vector<uint8_t>& data) {

        }
};



// PI4IOE5V6416 ioex;

class RoboxIoExpander {
    public:

        RoboxIoExpander(uint8_t address);
        ~RoboxIoExpander();

        uint8_t get_inputs(uint8_t port);
        // void configure_outputs(uint8_t port, uint8_t data);
        // uint8_t get_configure_outputs(uint8_t port);
        void set_output(uint8_t port, uint8_t data, uint8_t mask);
        static ExpanderConfig get_default_config();
        void io_configure();
        void register_observer(IoInterface* observer, ExpanderConfig observer_config);
        void loop();

    private:
        const uint8_t io_address;
        std::vector<ExpanderConfig> config;
        std::vector<IoInterface*> observers;

        uint8_t _out_buffer[2];
        // InterruptData interrupt_data;
        // uint8_t reset_pin;
        // SoftwareWire myWire;

        void io_set(uint8_t address, uint8_t data);
        void io_set_multi(uint8_t reg_address, std::vector<uint8_t> data);
        uint8_t io_get(uint8_t reg_address);
        std::vector<uint8_t> io_get_multi(uint8_t reg_address, uint8_t num_bytes);
        void io_reset();

};

#endif  // IOEXPANDER_H
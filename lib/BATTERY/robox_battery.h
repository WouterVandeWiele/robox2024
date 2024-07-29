#include "robox_io.h"


class RoboxBattery: public IoInterface {
    public:
        RoboxBattery(RoboxIoExpander* io);

        // static method that returns the IO expander config expected by this component
        static ExpanderConfig io_config();
        uint32_t battery_voltage();

        // interrupt method to be triggered by the io-expander
        void io_interrupt_observer(std::vector<uint8_t>& data) override;


    private:
        RoboxIoExpander* io;

};
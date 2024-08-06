#include "robox_io.h"


class RoboxMotor: public IoInterface {
    public:
        RoboxMotor(RoboxIoExpander* io);

        void init();

        void set_speed(float m1, float m2);
        void set_direction(bool m1, bool m2);

        void enable(bool enable);
        void shutdown(bool shutdown);

        bool is_shutdown();

        // static method that returns the IO expander config expected by this component
        static ExpanderConfig io_config();

        // interrupt method to be triggered by the io-expander
        void io_interrupt_observer(std::vector<uint8_t>& data) override;


    private:
        RoboxIoExpander* io;

};

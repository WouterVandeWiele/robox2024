#pragma once
#include "robox_io.h"

enum MotorMoves {
    mm_forward,
    mm_reverse,
    mm_center_left,
    mm_center_right,
    mm_left_forward_only,
    mm_left_reverse_only,
    mm_right_forward_only,
    mm_right_reverse_only,
    mm_last = mm_right_reverse_only,
};


class RoboxMotor: public IoInterface {
    public:
        RoboxMotor(RoboxIoExpander* io);

        void init();

        void set_speed(float m1, float m2);
        void set_direction(bool m1, bool m2);

        void enable(bool enable);
        void shutdown(bool shutdown);
        void shutdown_idempotent();
        void enable_idempotent();
        bool is_shutdown();

        void random_move();

        // static method that returns the IO expander config expected by this component
        static ExpanderConfig io_config();

        // interrupt method to be triggered by the io-expander
        void io_interrupt_observer(std::vector<uint8_t>& data) override;


    private:
        RoboxIoExpander* io;
        bool is_motor_on;
        uint32_t last_random_move;
};

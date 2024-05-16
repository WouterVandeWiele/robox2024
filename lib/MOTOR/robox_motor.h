
class RoboxMotor {
    public:
        RoboxMotor();

        void init();

        void set_speed(float m1, float m2);
        void set_direction(bool m1, bool m2);

        void enable(bool enable);
        void shutdown(bool shutdown);

};
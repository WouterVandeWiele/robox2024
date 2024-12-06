#pragma once
#include "robox_io.h"

extern QueueHandle_t xQueueBattery;

enum BatteryState {
    battery_high,
    battery_low,
    battery_verylow,
};

typedef struct BatteryData{
    uint32_t voltage;
    bool chargerCharging;
    bool chargerChgStBy;
    BatteryState state;
} BatteryData;

class RoboxBattery: public IoInterface {
    public:
        RoboxBattery(RoboxIoExpander* io);

        // static method that returns the IO expander config expected by this component
        static ExpanderConfig io_config();
        uint32_t battery_voltage();
        // uint8_t batteryChargerStatusCharging();
        // uint8_t batteryChargerStatusChgStBy();
        // interrupt method to be triggered by the io-expander
        void io_interrupt_observer(std::vector<uint8_t>& data) override;
        void initBattery();

    private:
        RoboxIoExpander* io;

};

#ifndef BLE_H
#define BLE_H

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
// #include "i2s.h"
#include "general_definitions.h"
#include "mux_interface.h"


#define A2DP_BLE_NAME "a2dp-i2s"


// callback used by A2DP to provide the sound data
void avrc_metadata_callback(uint8_t id, const uint8_t *text);


class RoboxBluetooth : public MuxInterface {
    public:
        RoboxBluetooth(): i2s(I2S_PIN_MUTE) {};

        // RoboxBluetooth(): a2dp_sink(), i2s(I2S_PIN_MUTE) {};
        // RoboxBluetooth();

        /*
        BLE Audio MUX controls
        */

        void setup();

        void mux_stop();
        void mux_start();

    private:
        I2SStream i2s;
        BluetoothA2DPSink a2dp_sink;
};


#endif  // BLE_H
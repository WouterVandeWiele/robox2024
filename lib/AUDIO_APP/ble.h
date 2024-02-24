#ifndef BLE_H
#define BLE_H

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include "mux_interface.h"
#include "pin_definitions.h"



#define A2DP_BLE_NAME "a2dp-i2s"


// callback used by A2DP to provide the sound data
void avrc_metadata_callback(uint8_t id, const uint8_t *text);


class RoboxBluetooth : public MuxInterface {
    public:
        // RoboxBluetooth();

        RoboxBluetooth(): i2s(I2S_PIN_MUTE), a2dp_sink() {};

        /*
        BLE Audio MUX controls
        */

        void setup();

        void mux_stop();
        void mux_start();

    private:
        BluetoothA2DPSink a2dp_sink;
        I2SStream i2s;

};


#endif  // BLE_H
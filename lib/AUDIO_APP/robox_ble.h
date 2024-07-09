#ifndef BLE_H
#define BLE_H

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include "robox_i2s.h"
#include "general_definitions.h"
#include "robox_mux_interface.h"


#define A2DP_BLE_NAME "a2dp-i2s"


// callback used by A2DP to provide the sound data
void avrc_metadata_callback(uint8_t id, const uint8_t *text);


class RoboxBluetooth : public MuxInterface {
    public:
        RoboxBluetooth() {};

        /*
        BLE Audio MUX controls
        */

        void mux_start() override;
        void mux_stop() override;
        void volume(float level) override;

    private:
        BluetoothA2DPSink a2dp_sink;
};


#endif  // BLE_H
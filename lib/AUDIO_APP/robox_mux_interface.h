#pragma once
#include <vector>
#include "general_config.h"

#include "robox_motor.h"
extern RoboxMotor* motor;

class MuxInterface {
    public:

        MuxInterface(float &volume_level)
            : volume_level(volume_level)
        {};

        virtual void mux_stop() {

        }

        virtual void mux_start() {

        }

        virtual void mux_copy() {

        }

        virtual void volume(float level) {
            
        }

        virtual float get_volume() {
            return volume_level;
        }
        
        virtual void volume_increment() {
            volume_level = min(volume_level + VOLUME_STEP, 1.f);

            volume(volume_level);
        }

        virtual void volume_decrement() {
            volume_level = max(volume_level - VOLUME_STEP, 0.f);

            volume(volume_level);    
        }

        virtual bool audio_active() {
            return false;
        }

        virtual void audio_play() {

        }

        virtual void audio_pause() {

        }

        virtual void audio_play_pause() {
            if (audio_active() == true) {
                audio_pause();

                motor->shutdown_idempotent();
            }
            else {
                audio_play();
                motor->enable_idempotent();
            }
        }

        virtual void audio_next() {

        }

        virtual void audio_previous() {
            
        }

        virtual std::vector<String> song_list() {
            std::vector<String> empty = {};
            return empty;
        }

    protected:
        float &volume_level;
};

#ifndef MUX_INTERFACE_H
#define MUX_INTERFACE_H

#include "general_config.h"

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
        
        virtual void volume_increment() {
            volume_level += VOLUME_STEP;

            volume(volume_level);
        }

        virtual void volume_decrement() {
            volume_level -= VOLUME_STEP;

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
            }
            else {
                audio_play();
            }
        }

    protected:
        float &volume_level;
};


#endif  // MUX_INTERFACE_H
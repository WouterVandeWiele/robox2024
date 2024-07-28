#ifndef ROBOX_VOID_H
#define ROBOX_VOID_H

#include "robox_mux_interface.h"

class RoboxVoid : public MuxInterface {
	public:
		RoboxVoid(bool beat_led, float &volume_level)
				: MuxInterface(volume_level)
			{};
};

#endif
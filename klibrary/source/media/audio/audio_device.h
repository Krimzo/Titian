#pragma once

#include "media/audio/audio.h"


namespace kl {
	class AudioDevice
	{
	public:
		UINT id = 0;

		AudioDevice(UINT id = 0);

		bool record_audio(Audio* output) const;
		bool play_audio(const Audio& audio) const;

		static UINT input_devices_count();
		static UINT output_devices_count();
	};
}

#pragma once

#include "media/audio/audio.h"


namespace kl {
	struct AudioDevice
	{
		UINT id = 0;

		AudioDevice(UINT id = 0);

		bool record_audio(Audio* output, const std::function<bool()>& should_record = [] { return true; }) const;
		bool play_audio(const Audio& audio, const std::function<bool()>& should_play = [] { return true; }) const;

		static UINT input_devices_count();
		static UINT output_devices_count();
	};
}

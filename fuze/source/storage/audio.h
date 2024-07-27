#pragma once

#include "titian.h"


namespace titian {
	using RAWAudio = kl::Audio;
	using RAWAudioType = kl::AudioType;
}

namespace titian {
	class Audio
	{
	public:
		RAWAudio audio;

		Audio();
		Audio(const String& path);

		float duration() const;

		float sample(float time) const;
	};
}

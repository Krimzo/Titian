#pragma once

#include "titian.h"


namespace titian {
	using RAWAudio = kl::Audio;
	using RAWAudioType = kl::AudioType;
}

namespace titian {
	class Audio : kl::NoCopy
	{
	public:
		RAWAudio out_audio;

		Audio();
		Audio(const String& path);

		float duration() const;

		float sample(float time) const;
	};
}

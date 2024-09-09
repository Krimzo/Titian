#pragma once

#include "titian.h"


namespace titian {
	using RAWAudio = kl::Audio;
	using RAWAudioType = kl::AudioType;
}

namespace titian {
	struct Audio : kl::NoCopy
	{
		RAWAudio out_audio;

		Audio();
		Audio(const StringView& path);

		float duration() const;
		float sample(float time) const;

		Ref<Audio> make_copy() const;
	};
}

#pragma once

#include "storage/audio.h"


namespace titian {
	class AudioEffect : kl::NoCopy
	{
	public:
		AudioEffect() = default;
		virtual void apply(Audio& audio) const = 0;
	};
}

#pragma once

#include "titian.h"


namespace titian {
	class AudioEffect
	{
	public:
		AudioEffect() = default;

		AudioEffect(const AudioEffect&) = delete;
		AudioEffect(AudioEffect&&) = delete;

		void operator=(const AudioEffect&) = delete;
		void operator=(AudioEffect&&) = delete;

		virtual void apply(Audio& audio) const = 0;
	};
}

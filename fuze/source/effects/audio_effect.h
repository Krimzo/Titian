#pragma once

#include "effects/effect_package.h"
#include "storage/audio.h"


namespace titian {
	class AudioEffect : kl::NoCopy
	{
	public:
		AudioEffect();
		virtual ~AudioEffect();

		virtual String get_name() const = 0;
		virtual void display_gui() = 0;

		virtual void apply(const EffectPackage& package, Audio& audio) = 0;
		
		virtual Ref<AudioEffect> make_copy() const = 0;
	};
}

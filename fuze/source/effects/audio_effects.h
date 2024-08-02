#pragma once

#include "effects/audio_effect.h"


namespace titian {
	class AudioEffectVolume : public AudioEffect
	{
	public:
		float volume = 1.0f;

		AudioEffectVolume();

		String get_name() const override;
		void display_gui() override;

		void apply(const EffectPackage& package, Audio& audio) override;

		Ref<AudioEffect> make_copy() const override;
	};
}

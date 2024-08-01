#pragma once

#include "effects/image_effect.h"
#include "effects/audio_effect.h"


namespace titian {
	enum class MediaType
	{
		IMAGE = 0,
		AUDIO = 1,
		VIDEO = 2,
	};
}

namespace titian {
	class Media : kl::NoCopy
	{
	public:
		String name;
		MediaType type = MediaType::IMAGE;
		float duration = 0.0f;

		Ref<Image> image;
		Ref<Audio> audio;
		Ref<Video> video;

		Vector<Ref<ImageEffect>> image_effects;
		Vector<Ref<AudioEffect>> audio_effects;

		Frame out_frame;
		Audio out_audio;

		Media();

		bool has_image() const;
		bool has_audio() const;
		bool has_video() const;

		void store_frame(const EffectPackage& package);
		void store_audio(const EffectPackage& package);

	private:
		void store_raw_frame(float time);
		void store_raw_audio();

		void apply_image_effects(const EffectPackage& package);
		void apply_audio_effects(const EffectPackage& package);
	};
}

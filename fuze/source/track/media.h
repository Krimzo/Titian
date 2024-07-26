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
	class Media
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

		Media(const Media&) = delete;
		Media(Media&&) = delete;

		void operator=(const Media&) = delete;
		void operator=(Media&&) = delete;

		bool get_frame(float time);
		void get_audio();

	private:
		bool get_raw_frame(float time, Frame& out);
		void get_raw_audio(Audio& out);

		void apply_image_effects(float time, Frame& out) const;
		void apply_audio_effects(Audio& out) const;
	};
}

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
		Ref<kl::VideoReader> video;

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
		bool get_audio(float time, float duration);

	private:
		std::unordered_map<int, Image> m_cached_frames;

		bool get_raw_frame(float time, Frame& out) const;
		bool get_raw_audio(float time, float duration, Audio& out) const;

		void apply_image_effects(float time, Frame& out) const;
		void apply_audio_effects(float time, float duration, Audio& out) const;
	};
}

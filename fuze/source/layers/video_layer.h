#pragma once

#include "track/track.h"


namespace titian {
	class VideoLayer : public Layer
	{
	public:
		Vector<Ref<Track>> tracks;
		float current_time = 0.0f;

		bool timeline_seconds = true;
		bool timeline_10seconds = true;
		bool timeline_minutes = true;

		dx::ComputeShader clear_shader;
		dx::ComputeShader mix_shader;

		VideoLayer();

		void init();
		bool update() override;

		void play();
		void stop();
		bool playing() const;

		float start_time() const;
		float end_time() const;

		void get_frame(Frame& out) const;

		void load_image(const String& path);
		void load_audio(const String& path);
		void load_video(const String& path);

	private:
		bool m_playing = false;
		float m_last_time = 0.0f;

		std::future<void> m_audio_worker;
		kl::AudioDevice m_audio_device{ 0 };
		Audio m_audio{ 96000 };
		
		void mix_frame(Frame& first, const Frame& second) const;

		void play_audio();
		void prepare_audio();
	};
}

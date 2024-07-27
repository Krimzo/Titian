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

		Ref<Track> selected_track;
		Ref<Media> selected_media;

		dx::ComputeShader clear_shader;
		dx::ComputeShader mix_shader;

		Int2 viewport_size;
		Frame out_frame;

		VideoLayer();

		void init();
		bool update() override;

		void play();
		void stop();
		bool playing() const;

		float start_time() const;
		float end_time() const;

		void store_frame();

		void load_image(const String& path);
		void load_audio(const String& path);
		void load_video(const String& path);

	private:
		bool m_playing = false;
		float m_last_time = 0.0f;

		std::future<void> m_audio_worker;
		kl::AudioDevice m_audio_device{ 0 };
		RAWAudio m_audio{ 96000 };
		
		void clear_frame();
		void mix_frame(const Frame& frame) const;

		void play_audio();
		void prepare_audio();
	};
}

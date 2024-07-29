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

		void load_file(const String& path);
		void load_image(const String& path);
		void load_audio(const String& path);
		void load_video(const String& path);

		int find_track(const Ref<Track>& track) const;
		void delete_track(const Ref<Track>& track);
		void move_track_up(const Ref<Track>& track);
		void move_track_down(const Ref<Track>& track);

		Ref<Track> find_track(const Ref<Media>& media) const;
		void delete_media(const Ref<Media>& media);
		float get_offset(const Ref<Media>& media) const;
		void update_offset(const Ref<Media>& media, float offset);
		void split_audio(Ref<Media>& media);

	private:
		bool m_playing = false;
		float m_last_time = 0.0f;

		dx::ComputeShader m_clear_shader;
		dx::ComputeShader m_mix_shader;

		std::future<void> m_audio_worker;
		kl::AudioDevice m_audio_device{ 0 };
		RAWAudio m_audio{ 96000 };
		
		void clear_frame() const;
		void mix_frame(const Frame& frame) const;

		void play_audio();
		void prepare_audio();
	};
}

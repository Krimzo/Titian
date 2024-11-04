#pragma once

#include "render/frame_handler.h"
#include "render/fuze_renderer.h"


namespace titian {
	struct VideoLayer : Layer_T<VideoLayer>
	{
		Vector<Ref<Track>> tracks;
		float current_time = 0.0f;

		bool timeline_seconds = true;
		bool timeline_10seconds = true;
		bool timeline_minutes = true;

		Ref<Track> selected_track;
		Ref<Media> selected_media;

		VideoLayer();

		bool update() override;

		void play();
		void stop();
		bool playing() const;

		void start_rendering(
			Int2 frame_size,
			int fps,
			int video_bit_rate,
			int audio_sample_rate);
		void stop_rendering();
		bool rendering() const;

		float render_progress() const;

		bool can_edit() const;

		float start_time() const;
		float end_time() const;

		void store_frame(Int2 size);
		Int2 frame_size() const;
		dx::ShaderView get_shader_view() const;
		void retrieve_frame(RAWImage& out_image) const;
		
		void load_file(const StringRef& path);
		void load_image(const StringRef& path);
		void load_audio(const StringRef& path);
		void load_video(const StringRef& path);

		int find_track(const Track& track) const;
		void delete_track(const Track& track);
		void move_track_up(const Track& track);
		void move_track_down(const Track& track);

		Ref<Track> find_track(const Media& media) const;
		void delete_media(const Media& media);
		float get_offset(const Media& media) const;
		void update_offset(Ref<Media> media, float offset);
		void split_audio(Media& media);

	private:
		bool m_playing = false;
		float m_last_time = 0.0f;

		FrameHandler m_handler;
		Ref<FuzeRenderer> m_renderer;

		std::future<void> m_audio_worker;
		kl::AudioDevice m_audio_device{ 0 };
		RAWAudio m_audio{ 48000 };

		static Opt<kl::VideoType> classify_video_format(const StringRef& path);
		static Opt<kl::AudioType> classify_audio_format(const StringRef& path);

		void play_audio();
		void prepare_audio();
	};
}

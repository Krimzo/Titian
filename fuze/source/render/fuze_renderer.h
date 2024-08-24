#pragma once

#include "track/track.h"
#include "render/frame_handler.h"


namespace titian {
	class FuzeRenderer
	{
	public:
		Vector<Ref<Track>> tracks;

		FuzeRenderer(
			const StringView& filepath,
			const kl::VideoType& video_type,
			const Int2& frame_size,
			int fps,
			int video_bit_rate,
			int audio_sample_rate);
		FuzeRenderer(
			const StringView& filepath,
			kl::AudioType audio_type,
			int audio_sample_rate);
		~FuzeRenderer();

		void load_tracks(const Vector<Ref<Track>>& tracks);
		void load_audio();
		void load_frame();

		float progress() const;

	private:
		String m_path;

		float m_current_time = 0.0f;
		float m_progress = 0.0f;

		Ref<kl::VideoWriter> m_video_writer;
		Ref<kl::Audio> m_audio_writer;

		FrameHandler m_handler;
		RAWImage m_temp_image;

		Int2 m_frame_size = { 1, 1 };
		int m_fps = 30;
		kl::AudioType m_audio_type = kl::AudioType::MP3;

		float start_time() const;
		float end_time() const;
	};
}

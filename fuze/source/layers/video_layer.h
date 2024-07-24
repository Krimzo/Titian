#pragma once

#include "track/track.h"


namespace titian {
	class VideoLayer : public Layer
	{
	public:
		Vector<Ref<Track>> tracks;
		float current_time = 0.0f;
		float last_time = 0.0f;
		bool playing = false;

		bool timeline_seconds = true;
		bool timeline_10seconds = true;
		bool timeline_minutes = true;

		dx::ComputeShader clear_shader;
		dx::ComputeShader mix_shader;

		VideoLayer();

		void init();
		bool update() override;

		float start_time() const;
		float end_time() const;

		void get_frame(Frame& out) const;
		void get_audio(float duration, Audio& out) const;

		void load_image(const String& path);
		void load_audio(const String& path);
		void load_video(const String& path);

	private:
		void mix_frame(Frame& first, Frame& second) const;
		void mix_audio(Audio& first, Audio& second) const;
	};
}

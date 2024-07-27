#pragma once

#include "titian.h"


namespace titian {
	class Video : kl::NoCopy
	{
	public:
		RAWImage out_frame;

		Video(const String& path);
		~Video();

		float duration() const;

		void cache_frames(const Int2& size);
		void store_frame(float time);

	private:
		const String m_path;
		Int2 m_resolution = {};
		float m_duration = 0.0f;
		float m_fps = 0.0f;

		Map<int, Vector<byte>> m_frames;
		std::mutex m_frames_lock;
		std::thread m_thread;
	};
}

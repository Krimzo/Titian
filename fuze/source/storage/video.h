#pragma once

#include "titian.h"


namespace titian {
	class Video
	{
	public:
		static constexpr int MAX_WORK_COUNT = 2;
		float buffer_range = 10.0f;

		Video(const String& path);

		float duration_seconds() const;

		bool get_frame(float time, Image& out);

	private:
		const String m_path;
		Int2 m_resolution = {};
		float m_duration = 0.0f;
		float m_fps = 0.0f;

		Map<int, Vector<byte>> m_frames;
		Set<int> m_queued_frames;
		std::mutex m_frames_lock;

		WorkQueue m_work;
		std::atomic<int> m_current_work = 0;
	};
}

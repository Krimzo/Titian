#pragma once

#include "storage/image.h"


namespace titian {
	struct Video : kl::NoCopy
	{
		static constexpr int SECTION_THREAD_COUNT = 3;
		static constexpr float BUFFERING_LENGTH = 10.0f;

		Int2 cache_scale;
		RAWImage out_frame;

		Video(const StringView& path);
		~Video();

		float duration() const;

		void store_frame(float time, bool wait);
		void cache_frames(float time);

		void wait();
		Vector<Vector<Pair<bool, float>>> buffering_sections() const;

		Ref<Video> make_copy() const;

	private:
		const String m_path;
		Int2 m_resolution = {};
		float m_duration = 0.0f;
		float m_fps = 0.0f;

		Map<int, String> m_frames;
		std::mutex m_frames_lock;

		Vector<std::jthread> m_threads;
		Vector<bool> m_buffering_states;
		Vector<std::array<Pair<bool, float>, SECTION_THREAD_COUNT>> m_buffering_sections;
	};
}

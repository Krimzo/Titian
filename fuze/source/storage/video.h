#pragma once

#include "titian.h"


namespace titian {
	class Video
	{
	public:
		Video(const String& path);

		float duration_seconds() const;

		void cache_frames(const Int2& size);
		bool get_frame(float time, Image& out);

	private:
		const String m_path;
		Int2 m_resolution = {};
		float m_duration = 0.0f;
		float m_fps = 0.0f;

		Map<int, Vector<byte>> m_frames;
	};
}

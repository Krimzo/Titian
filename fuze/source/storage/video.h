#pragma once

#include "titian.h"


namespace titian {
	class Video
	{
	public:
		float buffer_range = 10.0f;

		Video(const String& path);

		float duration_seconds() const;

		bool get_frame(float time, Image& out);

	private:
		Ref<kl::VideoReader> m_reader;
		Map<int, Vector<byte>> m_frames;
	};
}

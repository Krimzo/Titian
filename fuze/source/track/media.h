#pragma once

#include "titian.h"


namespace titian {
	enum class MediaType
	{
		Image = 0,
		Audio = 1,
		Video = 2,
	};
}

namespace titian {
	class Media
	{
	public:
		String name;
		MediaType type = MediaType::Image;
		float duration = 0.0f;

		Media();

		Media(const Media&) = delete;
		Media(Media&&) = delete;

		void operator=(const Media&) = delete;
		void operator=(Media&&) = delete;
	};
}

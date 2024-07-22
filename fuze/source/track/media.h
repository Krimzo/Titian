#pragma once

#include "titian.h"


namespace fuze {
	using namespace titian;
}

namespace fuze {
	enum class MediaType
	{
		Image = 0,
		Audio = 1,
		Video = 2,
	};
}

namespace fuze {
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

#pragma once

#include "titian.h"


namespace titian {
	using RAWImage = kl::Image;
	using RAWImageType = kl::ImageType;
}

namespace titian {
	class Image
	{
	public:
		RAWImage out_frame;

		Image();
		Image(const String& path);

		void cache_frame(const Int2& size);
	
	private:
		RAWImage m_image;
	};
}

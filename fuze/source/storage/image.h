#pragma once

#include "titian.h"


namespace titian {
	using RAWImage = kl::Image;
	using RAWImageType = kl::ImageType;
}

namespace titian {
	class Image : kl::NoCopy
	{
	public:
		RAWImage out_frame;

		Image();
		Image(const StringView& path);

		void cache_frame(const Int2& size);

		Ref<Image> make_copy() const;
	
	private:
		RAWImage m_image;
	};
}

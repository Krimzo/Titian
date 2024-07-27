#pragma once

#include "storage/image.h"
#include "storage/frame.h"


namespace titian {
	class ImageEffect
	{
	public:
		ImageEffect() = default;

		ImageEffect(const ImageEffect&) = delete;
		ImageEffect(ImageEffect&&) = delete;

		void operator=(const ImageEffect&) = delete;
		void operator=(ImageEffect&&) = delete;

		virtual void apply(float time, Frame& frame) const = 0;
	};
}

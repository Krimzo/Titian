#pragma once

#include "storage/image.h"
#include "storage/frame.h"


namespace titian {
	class ImageEffect : kl::NoCopy
	{
	public:
		ImageEffect() = default;
		virtual void apply(float time, Frame& frame) const = 0;
	};
}

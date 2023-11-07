#pragma once

#include "klibrary.h"


namespace titian {
	class StatePackage
	{
	public:
		kl::dx::RasterState raster_state = nullptr;
		kl::dx::DepthState depth_state = nullptr;
		kl::RenderShaders shader_state = {};
		kl::dx::BlendState blend_state = nullptr;

		StatePackage() = default;
	};
}

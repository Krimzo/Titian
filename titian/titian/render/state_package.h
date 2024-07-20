#pragma once

#include "standard/standard.h"


namespace titian {
	class StatePackage
	{
	public:
		dx::RasterState raster_state = nullptr;
		dx::DepthState depth_state = nullptr;
		kl::RenderShaders shader_state = {};
		dx::BlendState blend_state = nullptr;

		StatePackage() = default;
	};
}

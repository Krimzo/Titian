#pragma once

#include "standard.h"


namespace titian {
	class StatePackage
	{
	public:
		dx::RasterState raster_state;
		dx::DepthState depth_state;
		kl::RenderShaders shader_state;
		dx::BlendState blend_state;

		StatePackage() = default;
	};
}

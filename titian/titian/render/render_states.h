#pragma once

#include "render/states/raster_states.h"
#include "render/states/depth_states.h"
#include "render/states/sampler_states.h"
#include "render/states/shader_states.h"
#include "render/states/blend_states.h"


namespace titian {
	class RenderStates
	{
	public:
		Ref<RasterStates> raster_states = nullptr;
		Ref<DepthStates> depth_states = nullptr;
		Ref<SamplerStates> sampler_states = nullptr;
		Ref<ShaderStates> shader_states = nullptr;
		Ref<BlendStates> blend_states = nullptr;

		RenderStates(kl::GPU* gpu);
	};
}

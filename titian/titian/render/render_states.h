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
		kl::Object<RasterStates> raster_states = nullptr;
		kl::Object<DepthStates> depth_states = nullptr;
		kl::Object<SamplerStates> sampler_states = nullptr;
		kl::Object<ShaderStates> shader_states = nullptr;
		kl::Object<BlendStates> blend_states = nullptr;

		RenderStates(kl::GPU* gpu);
	};
}

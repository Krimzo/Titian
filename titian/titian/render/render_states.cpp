#include "main.h"


titian::RenderStates::RenderStates(kl::GPU* gpu)
{
	raster_states = new RasterStates(gpu);
	depth_states = new DepthStates(gpu);
	sampler_states = new SamplerStates(gpu);
	shader_states = new ShaderStates(gpu);
	blend_states = new BlendStates(gpu);
}

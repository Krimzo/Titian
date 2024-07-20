#include "main.h"


titian::RenderPass::RenderPass(const String& name, const LayerPackage& package)
	: BenchmarkInfo(name)
	, LayerHolder(package)
{}

void titian::RenderPass::process()
{
	const TimeBomb _ = this->time_it();

	if (!is_renderable()) {
		return;
	}

	StatePackage package = get_state_package();
	bind_states(package);
	render_self(package);
	unbind_states();
}

void titian::RenderPass::bind_states(const StatePackage& package)
{
	const kl::GPU* gpu = &app_layer->gpu;

	gpu->bind_raster_state(package.raster_state);
	gpu->bind_depth_state(package.depth_state);
	gpu->bind_render_shaders(package.shader_state);
	gpu->bind_blend_state(package.blend_state);
}

void titian::RenderPass::unbind_states()
{
	const kl::GPU* gpu = &app_layer->gpu;

	gpu->unbind_raster_state();
	gpu->unbind_depth_state();
	gpu->unbind_render_shaders();
	gpu->unbind_blend_state();
}

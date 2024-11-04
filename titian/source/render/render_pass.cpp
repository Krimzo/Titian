#include "titian.h"


titian::RenderPass::RenderPass(const StringRef& name)
	: BenchmarkInfo(name)
{}

void titian::RenderPass::process(Camera* camera)
{
	const TimeBomb _ = bench_time_bomb();
	bench_reset();

	if (!camera)
		return;

	StatePackage package;
	package.camera = camera;
	state_package(package);
	if (!package.camera)
		return;

	bind_states(package);
	render_self(package);
	unbind_states();
}

void titian::RenderPass::bind_states(const StatePackage& package)
{
	kl::GPU& gpu = AppLayer::get().gpu;
	gpu.bind_raster_state(package.raster_state);
	gpu.bind_depth_state(package.depth_state);
	gpu.bind_blend_state(package.blend_state);
	gpu.bind_shaders(package.shaders);
}

void titian::RenderPass::unbind_states()
{
	kl::GPU& gpu = AppLayer::get().gpu;
	gpu.unbind_raster_state();
	gpu.unbind_depth_state();
	gpu.unbind_blend_state();
	gpu.unbind_shaders();
}

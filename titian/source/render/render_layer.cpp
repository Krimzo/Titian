#include "titian.h"


titian::RenderLayer::RenderLayer()
	: Layer("RenderLayer")
{}

void titian::RenderLayer::init()
{
	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;

	raster_states = new RasterStates(gpu);
	depth_states = new DepthStates(gpu);
	sampler_states = new SamplerStates(gpu);
	shader_states = new ShaderStates(gpu);
	blend_states = new BlendStates(gpu);

	screen_mesh = gpu->create_screen_mesh();
}

bool titian::RenderLayer::update()
{
	const TimeBomb _ = bench_time_bomb();

	AppLayer* app_layer = Layers::get<AppLayer>();
	kl::GPU* gpu = &app_layer->gpu;
	Scene* scene = &Layers::get<GameLayer>()->scene;

	gpu->clear_internal();

	const float animation_time = app_layer->timer.elapsed();
	for (auto& [_, animation] : scene->animations) {
		animation->update(scene, animation_time);
	}

	scene->helper_iterate_entities([&](const String& name, Entity* entity)
	{
		if (Camera* camera = dynamic_cast<Camera*>(entity)) {
			camera->clear_targets();
			if (camera->enabled && name != scene->main_camera_name) {
				const Int2 texture_size = camera->resolution();
				for (auto& pass : passes) {
					gpu->set_viewport_size(texture_size);
					pass->process(camera);
				}
				if (Texture* texture = scene->helper_get_texture(camera->target_texture_name)) {
					texture->copy_other(camera->screen_texture->graphics_buffer);
				}
			}
		}
	});

	if (Camera* camera = scene->get_casted<Camera>(scene->main_camera_name)) {
		camera->clear_targets();
		if (camera->enabled) {
			const Int2 texture_size = camera->resolution();
			for (auto& pass : passes) {
				gpu->set_viewport_size(texture_size);
				camera->update_aspect_ratio(texture_size);
				pass->process(camera);
			}
			if (Texture* texture = scene->helper_get_texture(camera->target_texture_name)) {
				texture->copy_other(camera->screen_texture->graphics_buffer);
			}
		}
	}

	const Int2 window_size = app_layer->window.size();
	gpu->bind_internal_views();
	gpu->set_viewport_size(window_size);
	return true;
}

void titian::RenderLayer::present() const
{
	Scene* scene = &Layers::get<GameLayer>()->scene;
	Camera* main_camera = scene->get_casted<Camera>(scene->main_camera_name);
	Layers::get<AppLayer>()->gpu.swap_buffers(main_camera ? main_camera->v_sync : true);
}

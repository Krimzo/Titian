#include "titian.h"


titian::RenderLayer::RenderLayer()
	: Layer_T("RenderLayer")
{
	kl::GPU& gpu = AppLayer::get().gpu;
	screen_mesh = gpu.create_screen_mesh();
}

bool titian::RenderLayer::update()
{
	const TimeBomb _ = bench_time_bomb();

	AppLayer& app_layer = AppLayer::get();
	kl::GPU& gpu = app_layer.gpu;
	Scene& scene = GameLayer::get().scene();

	gpu.clear_internal();

	const float animation_time = app_layer.timer.elapsed();
	for (auto& [_, animation] : scene.animations) {
		animation->update(scene, animation_time);
	}

	scene.helper_iterate_entities([&](const String& name, Entity* entity)
	{
		if (Camera* camera = dynamic_cast<Camera*>(entity)) {
			camera->clear_targets();
			if (camera->enabled && name != scene.main_camera_name) {
				const Int2 texture_size = camera->resolution();
				for (auto& pass : passes) {
					gpu.set_viewport_size(texture_size);
					pass->process(camera);
				}
				if (Texture* texture = scene.helper_get_texture(camera->target_texture_name)) {
					texture->copy_other(camera->screen_texture.texture);
				}
			}
		}
	});

	if (Camera* camera = scene.get_casted<Camera>(scene.main_camera_name)) {
		camera->clear_targets();
		if (camera->enabled) {
			const Int2 texture_size = camera->resolution();
			for (auto& pass : passes) {
				gpu.set_viewport_size(texture_size);
				camera->update_aspect_ratio(texture_size);
				pass->process(camera);
			}
			if (Texture* texture = scene.helper_get_texture(camera->target_texture_name)) {
				texture->copy_other(camera->screen_texture.texture);
			}
		}
	}

	const Int2 window_size = app_layer.window.size();
	gpu.bind_internal_views();
	gpu.set_viewport_size(window_size);
	return true;
}

void titian::RenderLayer::present() const
{
	kl::GPU& gpu = AppLayer::get().gpu;
	Scene& scene = GameLayer::get().scene();

	Camera* main_camera = scene.get_casted<Camera>(scene.main_camera_name);
	gpu.swap_buffers(main_camera ? main_camera->v_sync : true);
}

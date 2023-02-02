#include "render/render_chain.h"

#include "cbuffers/entity_render.h"


static const kl::float2 shadow_map_texel_size = kl::float2::splash(1.0f / shadow_map_resolution);

void render_scene(state_machine* state)
{
	state->gpu->bind_raster_state(state->raster_states["entity"]);
	state->gpu->bind_depth_state(state->depth_states["entity"]);

	state->gpu->bind_shaders(state->shaders["entity"]);
	
	state->gpu->bind_sampler_state(state->sampler_states["skybox"], 0);
	state->gpu->bind_sampler_state(state->sampler_states["shadow"], 1);
	state->gpu->bind_sampler_state(state->sampler_states["entity"], 2);

	state->gpu->bind_pixel_shader_view(state->scene->camera.skybox, 0);
	state->gpu->bind_pixel_shader_view(state->textures["shadow_map"], 1);

	entity_render_vs_cb vs_cb = {};
	vs_cb.w_matrix = {};
	vs_cb.vp_matrix = state->scene->camera.matrix();
	vs_cb.vp_light_matrix = state->scene->sun_matrix();

	entity_render_ps_cb ps_cb = {};
	ps_cb.object_color = {};
	ps_cb.object_data = {};

	ps_cb.camera_position = { state->scene->camera.position, 0 };
	ps_cb.light_data = { state->scene->sun_direction.normalize(), state->scene->ambient_light };

	ps_cb.shadow_data = { shadow_map_texel_size, 0, 0 };

	for (auto& entity : *state->scene) {
		if (!entity->mesh) { continue; }

		if (entity->material.color_map) {
			state->gpu->bind_pixel_shader_view(entity->material.color_map, 2);
			ps_cb.object_data.x = 1.0f;
		}

		vs_cb.w_matrix = entity->matrix();

		ps_cb.object_color = entity->material.color;
		ps_cb.object_data.y = entity->material.reflection_factor;

		ps_cb.object_data.z = entity->material.refraction_factor;
		ps_cb.object_data.w = entity->material.refraction_index;

		state->gpu->set_vertex_const_buffer(vs_cb);
		state->gpu->set_pixel_const_buffer(ps_cb);

		state->gpu->draw_vertex_buffer(entity->mesh);
	}
}

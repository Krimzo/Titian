#include "render/render_chain.h"

#include "cbuffers/entity_render.h"


void render_scene(state_machine* state)
{
	state->gpu->bind_depth_state(state->depth_states["entity"]);
	state->gpu->bind_shaders(state->shaders["entity"]);
	
	state->gpu->bind_sampler_state(state->sampler_states["entity"], 0);

	entity_render_vs_cb vs_cb = {};
	vs_cb.w_matrix = {};
	vs_cb.vp_matrix = state->scene->camera.matrix();
	vs_cb.vs_misc_data = {};

	entity_render_ps_cb ps_cb = {};
	ps_cb.object_color = {};
	ps_cb.sun_direction = kl::float4(state->scene->sun_direction.normalize(), 0);
	ps_cb.ps_misc_data = {};

	for (auto& entity : *state->scene) {
		if (entity->mesh.empty()) { continue; }

		if (entity->material.color_map) {
			state->gpu->bind_pixel_shader_view(entity->material.color_map, 0);
			ps_cb.ps_misc_data.x = 1.0f;
		}

		vs_cb.w_matrix = entity->matrix();
		ps_cb.object_color = entity->material.color;

		state->gpu->set_vertex_const_buffer(vs_cb);
		state->gpu->set_pixel_const_buffer(ps_cb);

		for (auto& mesh_part : entity->mesh) {
			state->gpu->draw_vertex_buffer(mesh_part);
		}
	}
}

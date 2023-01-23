#include "update/update_frame.h"

#include "cbuffers/entity_render.h"


void render_scene(state_machine* state)
{
	if (!state->scene) { return; }

	state->gpu->bind_shaders(state->shaders["entity_render"]);
	
	entity_render_vs_cb vs_cb = {};
	vs_cb.w_matrix = {};
	vs_cb.vp_matrix = state->scene->camera.matrix();
	vs_cb.vs_misc_data = {};

	entity_render_ps_cb ps_cb = {};
	ps_cb.object_color = {};
	ps_cb.sun_direction = kl::float4(state->scene->sun_direction.normalize(), 0);
	ps_cb.ps_misc_data = {};

	for (auto& entity : *state->scene) {
		if (!entity->mesh) { return; }

		vs_cb.w_matrix = entity->matrix();
		ps_cb.object_color = entity->color;

		state->gpu->set_vertex_const_buffer(vs_cb);
		state->gpu->set_pixel_const_buffer(ps_cb);
		state->gpu->draw_vertex_buffer(entity->mesh);
	}
}

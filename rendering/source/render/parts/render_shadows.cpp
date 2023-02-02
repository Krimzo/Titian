#include "render/render_chain.h"

#include "cbuffers/shadow_render.h"


void render_shadows(state_machine* state)
{
	const kl::int2 old_viewport = state->gpu->get_viewport();
	const kl::mat4 VP = state->scene->sun_matrix();

	state->gpu->set_viewport({ shadow_map_resolution, shadow_map_resolution });
	state->gpu->bind_targets({}, state->depth_views["shadow_map"]);
	state->gpu->clear_depth_view(state->depth_views["shadow_map"]);

	state->gpu->bind_raster_state(state->raster_states["shadow"]);
	state->gpu->bind_depth_state(state->depth_states["shadow"]);

	state->gpu->bind_shaders(state->shaders["shadow"]);

	shadow_render_vs_cb vs_cb = {};

	for (auto& entity : *state->scene) {
		if (!entity->mesh) { continue; }

		vs_cb.WVP = VP * entity->matrix();
		state->gpu->set_vertex_const_buffer(vs_cb);

		state->gpu->draw_vertex_buffer(entity->mesh);
	}

	state->gpu->bind_internal_targets();
	state->gpu->set_viewport(old_viewport);
}

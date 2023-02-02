#include "render/render_chain.h"


static const kl::int2 display_size = { 600, 600 };

void display_depth(state_machine* state)
{
	const kl::int2 old_viewport = state->gpu->get_viewport();

	state->gpu->set_viewport(display_size);

	state->gpu->bind_raster_state(state->raster_states["skybox"]);
	state->gpu->bind_depth_state(state->depth_states["skybox"]);

	state->gpu->bind_shaders(state->shaders["display_depth"]);

	state->gpu->bind_sampler_state(state->sampler_states["shadow"], 0);
	state->gpu->bind_pixel_shader_view(state->textures["shadow_map"], 0);

	state->gpu->draw_vertex_buffer(state->meshes["screen"]);

	state->gpu->set_viewport(old_viewport);
}

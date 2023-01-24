#include "render/render_chain.h"

#include "cbuffers/skybox_render.h"


void render_skybox(state_machine* state)
{
	state->gpu->bind_depth_state(state->depth_states["skybox"]);
	state->gpu->bind_shaders(state->shaders["skybox"]);

	state->gpu->bind_sampler_state(state->sampler_states["skybox"], 0);
	state->gpu->bind_pixel_shader_view(state->scene->camera.skybox, 0);
	
	skybox_render_vs_cb vs_cb = {};
	vs_cb.vp_matrix = state->scene->camera.matrix();
	
	state->gpu->set_vertex_const_buffer(vs_cb);
	state->gpu->draw_vertex_buffer(state->meshes["cube"]);
}

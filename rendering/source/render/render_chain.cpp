#include "render/render_chain.h"


void render_chain(state_machine* state)
{
	const kl::color background = state->scene ? state->scene->camera.background : kl::color();

	state->gpu->clear_internal_color((kl::float4) background);
	state->gpu->clear_internal_depth(1.0f);

	if (state->scene) {
		if (state->scene->camera.skybox) {
			render_skybox(state);
		}
		render_scene(state);
	}

	state->gpu->swap_buffers(true);
}

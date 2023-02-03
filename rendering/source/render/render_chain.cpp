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
		if (state->scene->ocean_mesh) {
			render_ocean(state);
		}

		render_shadows(state);
	
		render_scene(state);

		if (debug_display_depth) {
			display_depth(state);
		}
	}

	state->gpu->swap_buffers(v_sync);
}

#include "callbacks/main_callbacks.h"

#include "update/update_frame.h"


void render_chain(state_machine* state);

void update_callback(state_machine* state)
{
	update_physics(state);
	update_light(state);
	handle_camera(state);

	custom_update(state);

	handle_resizes(state);
	render_chain(state);

	finalize_frame(state);
}

void render_chain(state_machine* state)
{
	static const kl::color background = { 30, 30, 30 };

	state->gpu->clear_internal_color((kl::float4) background);
	state->gpu->clear_internal_depth(1.0f);

	render_scene(state);
	state->gpu->swap_buffers(true);
}

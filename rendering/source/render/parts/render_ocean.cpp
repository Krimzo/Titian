#include "render/render_chain.h"

#include "cbuffers/ocean_render.h"


static constexpr float wave_amplitude = 0.25f;
static constexpr float wave_direction_frequency_0 = 1.0f;
static constexpr float wave_direction_frequency_1 = 1.0f;
static constexpr float wave_shift = 1.0f;

void render_ocean(state_machine* state)
{
	state->gpu->bind_depth_state(state->depth_states["ocean"]);
	state->gpu->bind_shaders(state->shaders["ocean"]);
	state->gpu->bind_geometry_shader(state->shaders["ocean"].geometry_shader);

	ocean_render_vs_cb vs_cb = {};
	vs_cb.w_matrix = {};
	vs_cb.vp_matrix = state->scene->camera.matrix();
	vs_cb.time_data = { state->timer.get_elapsed(), state->timer.get_interval(), 0.0f, 0.0f };
	vs_cb.ocean_data = { wave_amplitude, wave_direction_frequency_0, wave_direction_frequency_1, wave_shift };

	ocean_render_ps_cb ps_cb = {};
	ps_cb.object_color = (kl::float4) kl::colors::white;
	ps_cb.sun_direction = kl::float4(state->scene->sun_direction.normalize(), 0);

	state->gpu->set_vertex_const_buffer(vs_cb);
	state->gpu->set_pixel_const_buffer(ps_cb);
	state->gpu->draw_vertex_buffer(state->scene->ocean_mesh);

	state->gpu->bind_geometry_shader(nullptr);
}

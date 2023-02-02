#include "setup/setup_state.h"


std::string parse_shader(const std::string& name);

void setup_shaders(state_machine* state)
{
	auto skybox_shaders = parse_shader("skybox_render.hlsl");
	state->shaders["skybox"] = state->gpu->new_shaders(skybox_shaders);
	
	auto ocean_shaders = parse_shader("ocean_render.hlsl");
	state->shaders["ocean"] = state->gpu->new_shaders(ocean_shaders);
	state->shaders["ocean"].geometry_shader = state->gpu->new_geometry_shader(ocean_shaders);

	auto shadow_shaders = parse_shader("shadow_render.hlsl");
	state->shaders["shadow"] = state->gpu->new_shaders(shadow_shaders);

	auto display_depth_shaders = parse_shader("depth_render.hlsl");
	state->shaders["display_depth"] = state->gpu->new_shaders(display_depth_shaders);

	auto render_shaders = parse_shader("entity_render.hlsl");
	state->shaders["entity"] = state->gpu->new_shaders(render_shaders);
}

std::string parse_shader(const std::string& name)
{
	return kl::files::read_string(resouce_folder + "shaders/" + name);
}

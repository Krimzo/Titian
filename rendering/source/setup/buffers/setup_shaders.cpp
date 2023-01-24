#include "setup/setup_state.h"


std::string parse_shader(const std::string& name);

void setup_shaders(state_machine* state)
{
	auto render_shaders = parse_shader("entity_render.hlsl");
	state->shaders["entity"] = state->gpu->new_shaders(render_shaders);

	auto skybox_shaders = parse_shader("skybox_render.hlsl");
	state->shaders["skybox"] = state->gpu->new_shaders(skybox_shaders);
}

std::string parse_shader(const std::string& name)
{
	return kl::files::read_string(resouce_folder + "shaders/" + name);
}

#include "setup/setup_state.h"


kl::dx::texture parse_texture(kl::ref<kl::gpu> gpu, const std::string& name);

void setup_textures(state_machine* state)
{
	auto checkers_texture = parse_texture(state->gpu, "checkers.png");
	state->textures["checkers"] = state->gpu->new_shader_view(checkers_texture);

	auto dogo_texture = parse_texture(state->gpu, "dogo.png");
	state->textures["dogo"] = state->gpu->new_shader_view(dogo_texture);
}

kl::dx::texture parse_texture(kl::ref<kl::gpu> gpu, const std::string& name)
{
	return gpu->new_texture(kl::image(resouce_folder + "textures/" + name));
}

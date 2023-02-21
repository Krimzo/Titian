#include "setup/setup_state.h"


kl::dx::texture parse_skybox(kl::ref<kl::gpu> gpu, const std::string& name);

void setup_skyboxes(state_machine* state)
{
    auto clouds_skybox = parse_skybox(state->gpu, "clouds.png");
    state->skyboxes["clouds"] = state->gpu->create_shader_view(clouds_skybox, nullptr);

    auto night_skybox = parse_skybox(state->gpu, "night.png");
    state->skyboxes["night"] = state->gpu->create_shader_view(night_skybox, nullptr);
}

kl::dx::texture parse_skybox(kl::ref<kl::gpu> gpu, const std::string& name)
{
    kl::image skybox = kl::image(resouce_folder + "skyboxes/" + name);
    kl::error_check(skybox.width() % 4 || skybox.height() % 3, kl::format("Bad skybox \"", name, "\" aspect ratio"));

    const int part_size = skybox.width() / 4;

    kl::image box_sides[6] = {};
    box_sides[0] = skybox.get_rectangle(kl::int2(1, 1) * part_size, kl::int2(2, 2) * part_size);
    box_sides[1] = skybox.get_rectangle(kl::int2(3, 1) * part_size, kl::int2(4, 2) * part_size);
    box_sides[2] = skybox.get_rectangle(kl::int2(0, 1) * part_size, kl::int2(1, 2) * part_size);
    box_sides[3] = skybox.get_rectangle(kl::int2(2, 1) * part_size, kl::int2(3, 2) * part_size);
    box_sides[4] = skybox.get_rectangle(kl::int2(1, 0) * part_size, kl::int2(2, 1) * part_size);
    box_sides[5] = skybox.get_rectangle(kl::int2(1, 2) * part_size, kl::int2(2, 3) * part_size);

    return gpu->create_cube_texture(box_sides[0], box_sides[1], box_sides[2], box_sides[3], box_sides[4], box_sides[5]);
}

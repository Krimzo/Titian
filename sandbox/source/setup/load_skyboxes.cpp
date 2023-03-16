#include "sandbox.h"


kl::dx::shader_view parse_skybox(editor_state* state, const std::string& path);

void load_skyboxes(editor_state* state)
{
    state->scene->textures["clouds"] = parse_skybox(state, "../preview/skyboxes/clouds.png");
    state->scene->textures["night"] = parse_skybox(state, "../preview/skyboxes/night.png");
}

kl::dx::shader_view parse_skybox(editor_state* state, const std::string& path)
{
    kl::image skybox = kl::image(path);
    kl::error_check(skybox.width() % 4 || skybox.height() % 3, kl::format("Bad skybox \"", path, "\" aspect ratio"));

    const int part_size = skybox.width() / 4;

    kl::image box_sides[6] = {};
    box_sides[0] = skybox.get_rectangle(kl::int2(1, 1) * part_size, kl::int2(2, 2) * part_size);
    box_sides[1] = skybox.get_rectangle(kl::int2(3, 1) * part_size, kl::int2(4, 2) * part_size);
    box_sides[2] = skybox.get_rectangle(kl::int2(0, 1) * part_size, kl::int2(1, 2) * part_size);
    box_sides[3] = skybox.get_rectangle(kl::int2(2, 1) * part_size, kl::int2(3, 2) * part_size);
    box_sides[4] = skybox.get_rectangle(kl::int2(1, 0) * part_size, kl::int2(2, 1) * part_size);
    box_sides[5] = skybox.get_rectangle(kl::int2(1, 2) * part_size, kl::int2(2, 3) * part_size);

    auto texture = state->gpu->create_cube_texture(box_sides[0], box_sides[1], box_sides[2], box_sides[3], box_sides[4], box_sides[5]);
    return state->gpu->create_shader_view(texture, nullptr);
}

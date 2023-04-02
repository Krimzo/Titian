#include "state/editor_state.h"


editor_state::editor_state()
{
    // Icon
    window->set_icon(builtin_path + "textures/titian.ico");

    // Default meshes
    default_meshes["cube"] = kl::make<kl::mesh>(scene.get(), gpu.get(), kl::files::parse_mesh(builtin_path + "meshes/cube.obj", true));
    default_meshes["sphere"] = kl::make<kl::mesh>(scene.get(), gpu.get(), kl::files::parse_mesh(builtin_path + "meshes/sphere.obj", true));
    default_meshes["capsule"] = kl::make<kl::mesh>(scene.get(), gpu.get(), kl::files::parse_mesh(builtin_path + "meshes/capsule.obj", true));
    default_meshes["monke"] = kl::make<kl::mesh>(scene.get(), gpu.get(), kl::files::parse_mesh(builtin_path + "meshes/monke.obj", true));

    // Default material
    default_materials["white"] = kl::make<kl::material>();
    default_materials["white"]->color = kl::colors::white;
}

editor_state::~editor_state()
{
    default_meshes.clear();
    default_materials.clear();
    scene = nullptr;
}

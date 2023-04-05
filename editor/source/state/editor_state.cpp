#include "state/editor_state.h"


editor_state::editor_state()
{
    window->set_icon(builtin_path + "textures/titian.ico");
    change_scene(kl::make<kl::scene>());
}

editor_state::~editor_state()
{
    change_scene(nullptr);
}

void editor_state::change_scene(kl::ref<kl::scene> scene)
{
    default_meshes.clear();
    default_materials.clear();

    this->scene = scene;
    if (!scene) {
        return;
    }

    // Default meshes
    default_meshes["cube"] = kl::make<kl::mesh>(gpu.get(), scene.get(), kl::files::parse_mesh(builtin_path + "meshes/cube.obj", true));
    default_meshes["sphere"] = kl::make<kl::mesh>(gpu.get(), scene.get(), kl::files::parse_mesh(builtin_path + "meshes/sphere.obj", true));
    default_meshes["capsule"] = kl::make<kl::mesh>(gpu.get(), scene.get(), kl::files::parse_mesh(builtin_path + "meshes/capsule.obj", true));
    default_meshes["monke"] = kl::make<kl::mesh>(gpu.get(), scene.get(), kl::files::parse_mesh(builtin_path + "meshes/monke.obj", true));

    // Default material
    default_materials["white"] = kl::make<kl::material>();
    default_materials["white"]->color = kl::colors::white;
}

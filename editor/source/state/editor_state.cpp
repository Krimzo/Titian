#include "state/editor_state.h"


editor_state::editor_state()
{
    window->set_icon(builtin_path + "textures/titian.ico");
    change_scene(new kl::scene());
}

editor_state::~editor_state()
{
    gui_state = nullptr;
    render_state = nullptr;
    logger_state = nullptr;
    change_scene(nullptr);
}

void editor_state::change_scene(kl::object<kl::scene> scene)
{
    default_meshes.clear();
    default_materials.clear();

    this->scene = scene;
    if (!scene) {
        return;
    }

    // Default meshes
    default_meshes["cube"] = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/cube.obj", true));
    default_meshes["sphere"] = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/sphere.obj", true));
    default_meshes["capsule"] = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/capsule.obj", true));
    default_meshes["monke"] = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/monke.obj", true));

    // Default material
    default_materials["white"] = new kl::material();
    default_materials["white"]->color = kl::colors::white;
}

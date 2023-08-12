#include "editor.h"


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
    default_mesh.cube = nullptr;
    default_mesh.sphere = nullptr;
    default_mesh.capsule = nullptr;
    default_mesh.monke = nullptr;
    default_material.white = nullptr;

    if (gui_state) {
        gui_state->mesh_editor.selected_mesh = nullptr;
        gui_state->material_editor.selected_material = nullptr;
        gui_state->material_editor.selected_texture = nullptr;
    }

    this->scene = scene;
    if (!scene) {
        return;
    }

    // Default meshes
    default_mesh.cube = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/cube.obj", true));
    default_mesh.sphere = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/sphere.obj", true));
    default_mesh.capsule = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/capsule.obj", true));
    default_mesh.monke = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/monke.obj", true));

    // Default material
    default_material.white = new kl::material();
    default_material.white->color = kl::colors::white;
}

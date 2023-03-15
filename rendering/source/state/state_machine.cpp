#include "state/state_machine.h"


state_machine::state_machine()
{
    window = kl::make<kl::window>(kl::int2(1600, 900), "Fun");
    gpu = kl::make<kl::gpu>((HWND) *window);

    render_state = kl::make<::render_state>(gpu, window->size());
    scene = kl::make<kl::scene>();

    // Default meshes
    default_meshes["cube"] = kl::ref<kl::mesh>(new kl::mesh(*gpu, *scene, kl::files::parse_mesh(resouce_folder + "/meshes/cube.obj", true)));
    default_meshes["sphere"] = kl::ref<kl::mesh>(new kl::mesh(*gpu, *scene, kl::files::parse_mesh(resouce_folder + "/meshes/sphere.obj", true)));
    default_meshes["capsule"] = kl::ref<kl::mesh>(new kl::mesh(*gpu, *scene, kl::files::parse_mesh(resouce_folder + "/meshes/capsule.obj", true)));
    default_meshes["monke"] = kl::ref<kl::mesh>(new kl::mesh(*gpu, *scene, kl::files::parse_mesh(resouce_folder + "/meshes/monke.obj", true)));

    // Default material
    default_materials["white"] = kl::make<kl::material>();
    default_materials["white"]->color = kl::colors::white;
}

state_machine::~state_machine()
{
    default_meshes.clear();
    default_materials.clear();
    scene = nullptr;
}

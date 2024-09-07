#include "sandbox.h"


titian::SandboxPiecePrepare::SandboxPiecePrepare(TitianEditor* editor)
    : SandboxPiece(editor)
{}

void titian::SandboxPiecePrepare::setup_self()
{
    Scene* scene = &editor->game_layer.scene;
    kl::GPU* gpu = &editor->app_layer.gpu;

    // Camera
    Ref camera = new Camera(scene->physics(), false, gpu);
    camera->far_plane = 75.0f;
    camera->set_position({ 0.0f, 1.0f, -4.0f });
    camera->skybox_name = "sky";
    scene->main_camera_name = "default_camera";
    scene->add_entity(scene->main_camera_name, camera);

    // Ambient
    Ref ambient_light = new AmbientLight(scene->physics(), false);
    ambient_light->color = kl::colors::WHITE;
    ambient_light->intensity = 0.1f;
    scene->main_ambient_light_name = "default_ambient";
    scene->add_entity(scene->main_ambient_light_name, ambient_light);

    // Directional
    Ref directional_light = new DirectionalLight(scene->physics(), false, gpu);
    directional_light->set_direction({ 0.26f, -0.335f, 0.9f });
    scene->main_directional_light_name = "default_directional";
    scene->add_entity(scene->main_directional_light_name, directional_light);
}

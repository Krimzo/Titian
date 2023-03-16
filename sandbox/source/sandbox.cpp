#include "sandbox.h"


int sandbox_main()
{
    // Setup
    editor_state state = {};
    editor_setup(&state);

    // Test scene setup
    setup_preview_scene(&state);

    // Loop
    state.timer.reset();
    while (state.window->process(false)) {
        state.timer.update_interval();
        editor_update(&state);
    }
    return 0;
}

void setup_preview_scene(editor_state* state)
{
    // Load data
    load_meshes(state);
    load_textures(state);
    load_skyboxes(state);

    // Camera
    state->scene->camera = kl::make<kl::camera>();
    state->scene->camera->far_plane = 75.0f;
    state->scene->camera->position = { 0, 1, -4 };
    state->scene->camera->skybox = state->scene->textures["clouds"];

    // Light
    state->scene->ambient_light = kl::make<kl::ambient_light>();
    state->scene->directional_light = kl::ref<kl::directional_light>(new kl::directional_light(*state->gpu, 4096));
    state->scene->directional_light->set_direction({ 0.26f, -0.335f, 0.9f });

    // Parts
    setup_preview_monkes(state, 15);
    setup_preview_spheres(state, 7);
    setup_preview_cubes(state, 15);
    setup_preview_bullets(state, 15);
    setup_preview_physics_test(state, 9);
}

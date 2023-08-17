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
        state.timer.update_delta();
        editor_update(&state);
    }
    return 0;
}

void setup_preview_scene(editor_state* state)
{
    // script testing
    {
        jclass clazz = java::load_eternal_class("sdk/math/vector/Float3");
        if (clazz) {
            java::method constructor = java::get_constructor(clazz);
            java::method toString = java::get_method(clazz, "toString", java::build_class_signature("java/lang/String"));
            if (constructor && toString) {
                java::object instance = java::new_object(clazz, constructor);
                if (instance) {
                    java::object result = toString.call<jobject>(instance);
                    jstring result_str = reinterpret_cast<jstring>((jobject) result);
                    if (result_str) {
                        std::string real_result = java::read_string(result_str);
                        std::cout << real_result << std::endl;
                    }
                }
            }
        }
    }

    // Setup default meshes/materials
    state->scene->meshes["cube"] = state->default_mesh.cube;
    state->scene->meshes["sphere"] = state->default_mesh.sphere;
    state->scene->meshes["capsule"] = state->default_mesh.capsule;
    state->scene->meshes["monke"] = state->default_mesh.monke;
    state->scene->materials["white"] = state->default_material.white;

    // Load data
    load_meshes(state);
    load_textures(state);
    load_skyboxes(state);

    // Camera
    state->scene->camera->far_plane = 75.0f;
    state->scene->camera->origin = { 0, 1, -4 };
    state->scene->camera->skybox = state->scene->textures["clouds"];

    // Light
    state->scene->ambient_light = new kl::ambient_light();
    state->scene->directional_light = new kl::directional_light(&state->gpu, 4096);
    state->scene->directional_light->set_direction({ 0.26f, -0.335f, 0.9f });

    // Parts
    setup_preview_monkes(state, 15);
    setup_preview_spheres(state, 7);
    setup_preview_cubes(state, 15);
    setup_preview_physics_test(state, 9);
}

export module sandbox;

export import editor;

export inline const std::string RESOURCE_PATH = "./preview/";

export void load_meshes(EditorState* state);
export void load_textures(EditorState* state);
export void load_skyboxes(EditorState* state);

export void setup_preview_cubes(EditorState* state, int size);
export void setup_preview_spheres(EditorState* state, int size);
export void setup_preview_monkes(EditorState* state, int size);
export void setup_preview_physics_test(EditorState* state, int size);

void setup_preview_scene(EditorState* state);

export int sandbox_main()
{
    // Setup
    EditorState state = {};
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

void setup_preview_scene(EditorState* state)
{
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
    state->scene->ambient_light = new kl::AmbientLight();
    state->scene->directional_light = new kl::DirectionalLight(&state->gpu, 4096);
    state->scene->directional_light->set_direction({ 0.26f, -0.335f, 0.9f });

    // Parts
    setup_preview_monkes(state, 15);
    setup_preview_spheres(state, 7);
    setup_preview_cubes(state, 15);
    setup_preview_physics_test(state, 9);
}

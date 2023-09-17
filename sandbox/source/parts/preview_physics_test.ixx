export module preview_physics_test;

export import sandbox;

void setup_platform(EditorState* state);
void setup_objects(EditorState* state, int size);

export void setup_preview_physics_test(EditorState* state, const int size)
{
    setup_platform(state);
    setup_objects(state, size);
}

void setup_platform(EditorState* state)
{
    kl::Object platform = state->scene->make_entity(false);

    const kl::Float3 scale = { 15.0f, 0.1f, 15.0f };
    platform->render_scale = scale;

    platform->set_rotation({ 1.0f, 0.0f, 0.0f });
    platform->set_position({ 0.0f, -7.0f, -25.0f });
    platform->set_collider(state->scene->make_box_collider(scale));

    platform->mesh = state->default_mesh.cube;
    platform->material = state->default_material.white;

    state->scene->add("PhysicsTestPlatform", platform);
}

void setup_objects(EditorState* state, const int size)
{
    const int half_size = size / 2;
    const kl::Float3 scale = { 0.5f, 0.5f, 0.5f };
    int box_counter = 0;

    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            kl::Object box = state->scene->make_entity(true);

            box->render_scale = scale;
            box->set_rotation(kl::random::gen_float3(360.0f));
            box->set_position({ (half_size - x) * 2.25f, 15.0f, -25.0f + (half_size - z) * 2.25f });
            box->set_collider(state->scene->make_box_collider(scale));

            box->set_mass(2.5f);
            box->set_gravity(true);

            kl::Object material = new kl::Material();
            material->texture_blend = 0.5f;
            material->color = kl::random::gen_color();
            material->color_map = state->scene->textures["dogo"];
            material->normal_map = state->scene->textures["concrete_normal"];
            state->scene->materials[kl::format("box_mat_", box_counter)] = material;

            box->mesh = state->default_mesh.cube;
            box->material = material;

            state->scene->add(kl::format("Box", box_counter), box);
            box_counter += 1;
        }
    }
}

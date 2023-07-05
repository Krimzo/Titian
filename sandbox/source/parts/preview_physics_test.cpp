#include "sandbox.h"


void setup_platform(editor_state* state);
void setup_objects(editor_state* state, int size);

void setup_preview_physics_test(editor_state* state, const int size)
{
    setup_platform(state);
    setup_objects(state, size);
}

void setup_platform(editor_state* state)
{
    kl::object<kl::entity> platform = state->scene->make_entity(false);

    const kl::float3 scale = { 15.0f, 0.1f, 15.0f };

    platform->set_collider(state->scene->make_box_collider(scale));

    platform->render_scale = scale;
    platform->set_rotation({ 1.0f, 0.0f, 0.0f });
    platform->set_position({ 0.0f, -7.0f, -25.0f });

    platform->mesh = state->default_meshes["cube"];
    platform->material = state->default_materials["white"];

    state->scene->add("PhysicsTestPlatform", platform);
}

void setup_objects(editor_state* state, const int size)
{
    const int half_size = size / 2;
    const kl::float3 scale = { 0.5f, 0.5f, 0.5f };

    int box_counter = 0;

    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            kl::object<kl::entity> box = state->scene->make_entity(true);

            box->set_mass(2.5f);
            box->set_gravity(true);

            box->set_collider(state->scene->make_box_collider(scale));

            box->render_scale = scale;
            box->set_rotation(kl::random::get_float3(360.0f));
            box->set_position({ (half_size - x) * 2.25f, 15.0f, -25.0f + (half_size - z) * 2.25f });

            box->mesh = state->default_meshes["cube"];
            box->material = new kl::material();

            box->material->color_map = state->scene->textures["dogo"];
            box->material->normal_map = state->scene->textures["concrete_normal"];

            box->material->color = (kl::float4) kl::random::get_color();
            box->material->texture_blend = 0.5f;

            state->scene->add(kl::format("Box", box_counter), box);

            box_counter += 1;
        }
    }
}

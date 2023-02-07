#include "setup/scene/setup_scene.h"


void setup_platform(state_machine* state);
void setup_objects(state_machine* state, int size);

void setup_physics_test(state_machine* state, const int size)
{
    setup_platform(state);
    setup_objects(state, size);
}

void setup_platform(state_machine* state)
{
    kl::ref<kl::entity> platform = state->scene->make_entity(false);

    const kl::float3 scale = { 15.0f, 0.1f, 15.0f };

    platform->set_collider(state->scene->make_box_collider(scale));

    platform->render_scale = scale;
    platform->set_rotation({ 1.0f, 0.0f, 0.0f });
    platform->set_position({ 0.0f, -7.0f, -25.0f });

    platform->mesh = state->meshes["cube"];
    platform->material = kl::make<kl::material>();

    platform->material->color = kl::float4::splash(1.0f);

    state->scene->add(platform);
}

void setup_objects(state_machine* state, const int size)
{
    const int half_size = size / 2;

    const kl::float3 scale = { 0.5f, 0.5f, 0.5f };

    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            kl::ref<kl::entity> box = state->scene->make_entity(true);

            box->set_mass(2.5f);
            box->set_collider(state->scene->make_box_collider(scale));

            box->render_scale = scale;
            box->set_rotation(kl::random::get_float3(360.0f));
            box->set_position({ (half_size - x) * 2.25f, 15.0f, -25.0f + (half_size - z) * 2.25f });

            box->mesh = state->meshes["cube"];
            box->material = kl::make<kl::material>();

            box->material->color_map = state->textures["dogo"];
            box->material->normal_map = state->textures["concrete_normal"];

            box->material->color = (kl::float4) kl::random::get_color();
            box->material->texture_blend = 0.5f;

            state->scene->add(box);
        }
    }
}

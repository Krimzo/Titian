#include "setup/scene/setup_scene.h"


static constexpr float bullet_scale = 10.0f;
static constexpr float fire_velocity = 25.0f;

static const kl::float3 target_position = { 0.0f, -6.5f, -25.0f };

void setup_bullets(state_machine* state, const int size)
{
    const int half_size = size / 2;

    std::vector<std::function<void()>> all_callbacks = {};

    const kl::ref<kl::material> bullet_material = kl::make<kl::material>();
    bullet_material->color = (kl::float4) kl::color(232, 230, 227);
    bullet_material->reflection_factor = 0.15f;

    const kl::ref<kl::material> bullet_casing_material = kl::make<kl::material>();
    bullet_casing_material->color = (kl::float4) kl::color(210, 180, 130);
    bullet_casing_material->reflection_factor = 0.2f;

    const kl::ref<kl::material> casing_primer_material = kl::make<kl::material>();
    casing_primer_material->color = (kl::float4) kl::color(232, 230, 227);
    casing_primer_material->reflection_factor = 0.1f;

    for (int i = 0; i < size; i++) {
        // Bullet
        kl::ref<kl::entity> bullet = state->scene->make_entity(true);

        bullet->set_gravity(false);
        bullet->set_mass(1.0f);

        bullet->render_scale *= bullet_scale;

        kl::float3 old_position = bullet->get_position();
        old_position.x = (float) (i - half_size);
        old_position.y = -6.0f;
        bullet->set_position(old_position);

        static const std::function<kl::float3(const kl::float3&)> directional_vector_to_euler = [](const kl::float3& dir) -> kl::float3
        {
            static const kl::float2 start_dir_yz = { 1, 0 };
            const float rx = -start_dir_yz.angle({ dir.y, dir.z });
            const float ry = -kl::float2(0, dir.z).angle({ dir.x, dir.z }, true);
            return { rx, ry, 0 };
        };

        const kl::float3 target_direction = (target_position - bullet->get_position()).normalize();
        bullet->set_rotation(directional_vector_to_euler(target_direction));

        bullet->mesh = state->meshes["bmg_bullet"];
        bullet->material = bullet_material;

        bullet->set_collider(state->scene->make_mesh_collider(*bullet->mesh, bullet->render_scale));

        state->scene->add(kl::format("Bullet", i), bullet);

        // Bullet casing
        kl::ref<kl::entity> bullet_casing = state->scene->make_entity(false);

        bullet_casing->render_scale = bullet->render_scale;
        bullet_casing->set_rotation(bullet->get_rotation());
        bullet_casing->set_position(bullet->get_position());

        bullet_casing->mesh = state->meshes["bmg_casing"];
        bullet_casing->material = bullet_casing_material;

        state->scene->add(kl::format("BulletCasing", i), bullet_casing);

        // Casing primer
        kl::ref<kl::entity> casing_primer = state->scene->make_entity(false);

        casing_primer->render_scale = bullet_casing->render_scale;
        casing_primer->set_rotation(bullet_casing->get_rotation());
        casing_primer->set_position(bullet_casing->get_position());

        casing_primer->mesh = state->meshes["bmg_primer"];
        casing_primer->material = casing_primer_material;

        state->scene->add(kl::format("CasingPrimer", i), casing_primer);

        // Callbacks
        kl::ref<bool> bullet_fired = kl::make<bool>(false);

        all_callbacks.push_back([state, bullet, casing_primer, bullet_fired]
        {
            if (!*bullet_fired) {
                const kl::float4 default_bullet_direction = { 0.0f, 1.0f, 0.0f, 1.0f };
                auto correct_direction = (kl::mat4::rotation(bullet->get_rotation()) * default_bullet_direction).xyz;

                bullet->set_velocity(correct_direction.normalize() * fire_velocity);

                casing_primer->mesh = state->meshes["bmg_primer_fired"];

                *bullet_fired = true;
            }
        });
    }

    state->window->keyboard.space.on_release = [all_callbacks]
    {
        for (auto& callback : all_callbacks) {
            callback();
        }
    };
}

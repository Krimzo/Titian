export module preview_monkes;

export import sandbox;

void setup_preview_monkes(editor_state* state, const int size)
{
    static constexpr float x_offset = -16.0f;

    const int entity_count = size * size;
    const int half_size = size / 2;

    for (int i = 0; i < entity_count; i++) {
        auto monke = state->scene->make_entity(false);
        monke->render_scale = kl::float3(0.5f);

        const int x = i % size;
        const int y = i / size;

        monke->set_position({
            (float) (x - half_size) + x_offset,
            (float) (y - half_size),
            5.0f
            });

        const float percentage = (i + 1.0f) / entity_count;
        const float normalized = kl::clamp(percentage, 0.0f, 1.0f);

        kl::object material = new kl::material();
        material->color = kl::float4(normalized);
        state->scene->materials[kl::format("monke_mat_", i)] = material;

        monke->mesh = state->default_mesh.monke;
        monke->material = material;

        state->scene->add(kl::format("Monke", i), monke);
    }
}

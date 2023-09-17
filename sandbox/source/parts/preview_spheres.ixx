export module preview_spheres;

export import sandbox;

void setup_preview_spheres(editor_state* state, const int size)
{
    static constexpr float x_offset = 0.0f;

    const int half_size = size / 2;
    int sphere_counter = 0;

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            kl::object<kl::entity> sphere = state->scene->make_entity(false);
            sphere->set_position({ (x - half_size) * 2.25f + x_offset, (y - half_size) * 2.25f, 5.0f });

            kl::object material = new kl::material();
            material->color = kl::float4(1.0f);
            material->reflection_factor = (float) sphere_counter / (size * size);
            material->refraction_factor = (float) (size * size - sphere_counter) / (size * size);
            material->refraction_index = 1.0f / 1.52f;
            state->scene->materials[kl::format("sphere_mat_", sphere_counter)] = material;

            sphere->mesh = state->default_mesh.sphere;
            sphere->material = material;

            state->scene->add(kl::format("Sphere", sphere_counter), sphere);
            sphere_counter += 1;
        }
    }
}

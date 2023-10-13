export module preview_spheres;

export import sandbox;

void setup_preview_spheres(EditorState* state, const int size)
{
    static constexpr float x_offset = 0.0f;

    const int half_size = size / 2;
    int sphere_counter = 0;

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            kl::Object<kl::Entity> sphere = state->scene->make_entity(false);
            sphere->set_position({ (x - half_size) * 2.25f + x_offset, (y - half_size) * 2.25f, 5.0f });

            kl::Object material = new kl::Material();
            material->color = kl::Float4 {1.0f};
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

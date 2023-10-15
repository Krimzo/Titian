export module piece_spheres;

export import sandbox_piece;

export namespace titian {
    class SandboxPieceSpheres : public SandboxPiece
    {
    public:
        int size = 0;
        float x_offset = 0.0f;

        SandboxPieceSpheres(TitianEditor* editor, const int size)
            : SandboxPiece(editor)
        {
            this->size = size;
        }

        ~SandboxPieceSpheres() override
        {}

        void setup_self() override
        {
            Scene* scene = &editor->game_layer->scene;

            const int half_size = size / 2;
            int sphere_counter = 0;

            for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                    const std::string mesh_id = "sphere";
                    const std::string material_id = kl::format("sphere_mat_", sphere_counter);
                    const std::string entity_id = kl::format("Sphere", sphere_counter);
                    
                    // Mesh
                    scene->meshes[mesh_id] = scene->default_meshes->sphere;

                    // Material
                    kl::Object material = new Material();
                    material->color = kl::Float4{ 1.0f };
                    material->reflection_factor = (float) sphere_counter / (size * size);
                    material->refraction_factor = (float) (size * size - sphere_counter) / (size * size);
                    material->refraction_index = 1.0f / 1.52f;
                    scene->materials[material_id] = material;

                    // Entity
                    kl::Object sphere = scene->make_entity(false);
                    sphere->set_position({ (x - half_size) * 2.25f + x_offset, (y - half_size) * 2.25f, 5.0f });

                    sphere->mesh = mesh_id;
                    sphere->material = material_id;

                    scene->add(entity_id, sphere);
                    sphere_counter += 1;
                }
            }
        }
    };
}

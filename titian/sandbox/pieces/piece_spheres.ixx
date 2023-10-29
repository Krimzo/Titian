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
                    const std::string mesh_name = "sphere";
                    const std::string material_name = kl::format("sphere_mat_", sphere_counter);
                    const std::string entity_name = kl::format("Sphere", sphere_counter);
                    
                    // Mesh
                    scene->meshes[mesh_name] = scene->default_meshes->sphere;

                    // Material
                    kl::Object material = new Material();
                    material->color = kl::Float4{ 1.0f };
                    material->reflection_factor = (float) sphere_counter / (size * size);
                    material->refraction_factor = (float) (size * size - sphere_counter) / (size * size);
                    material->refraction_index = 1.0f / 1.52f;
                    scene->materials[material_name] = material;

                    // Entity
                    kl::Object sphere = scene->new_entity(false);
                    sphere->set_position({ (x - half_size) * 2.25f + x_offset, (y - half_size) * 2.25f, 5.0f });

                    sphere->mesh_name = mesh_name;
                    sphere->material_name = material_name;

                    scene->add_entity(entity_name, sphere);
                    sphere_counter += 1;
                }
            }
        }
    };
}

export module piece_monkes;

export import sandbox_piece;

export namespace titian {
    class SandboxPieceMonkes : public SandboxPiece
    {
    public:
        int size = 0;
        float x_offset = -16.0f;

        SandboxPieceMonkes(TitianEditor* editor, const int size)
            : SandboxPiece(editor)
        {
            this->size = size;
        }

        ~SandboxPieceMonkes() override
        {}

        void setup_self() override
        {
            Scene* scene = &editor->game_layer->scene;

            const int entity_count = size * size;
            const int half_size = size / 2;

            for (int i = 0; i < entity_count; i++) {
                const int x = i % size;
                const int y = i / size;

                const std::string mesh_name = "monke";
                const std::string material_name = kl::format("monke_mat_", i);
                const std::string entity_name = kl::format("Monke", i);

                // Mesh
                scene->meshes[mesh_name] = editor->game_layer->scene->default_meshes->monke;

                // Material
                const float percentage = (i + 1.0f) / entity_count;
                const float normalized = kl::clamp(percentage, 0.0f, 1.0f);

                kl::Object material = new Material();
                material->color = kl::Float4{ normalized };
                scene->materials[material_name] = material;

                // Entity
                kl::Object monke = scene->make_entity(false);
                monke->scale = kl::Float3{ 0.5f };
                monke->set_position({
                    (float) (x - half_size) + x_offset,
                    (float) (y - half_size),
                    5.0f
                });

                monke->mesh_name = mesh_name;
                monke->material_name = material_name;

                scene->add(entity_name, monke);
            }
        }
    };
}
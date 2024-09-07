#include "sandbox.h"


titian::SandboxPieceMonkes::SandboxPieceMonkes(TitianEditor* editor, const int size)
    : SandboxPiece(editor)
{
    this->size = size;
}

void titian::SandboxPieceMonkes::setup_self()
{
    Scene* scene = &editor->game_layer.scene;
    kl::GPU* gpu = &editor->app_layer.gpu;

    const int entity_count = size * size;
    const int half_size = size / 2;

    for (int i = 0; i < entity_count; i++) {
        const int x = i % size;
        const int y = i / size;

        const String mesh_name = "monke";
        const String animation_name = "monke";
        const String counter_id = kl::format(std::setw(3), std::setfill('0'), i);
        const String material_name = kl::format("monke_mat_", counter_id);
        const String entity_name = kl::format("Monke", counter_id);

        scene->meshes[mesh_name] = scene->default_meshes->monke;
		scene->animations[animation_name] = scene->default_animations->monke;

        // Material
        const float percentage = (i + 1.0f) / entity_count;
        const float normalized = kl::clamp(percentage, 0.0f, 1.0f);

        Ref material = new Material();
        material->color = Float4{ Float3{ normalized }, 1.0f };
        scene->materials[material_name] = material;

        // Entity
        Ref monke = scene->new_entity(false);
        monke->scale = Float3{ 0.5f };
        monke->set_rotation({ 0.0f, 180.0f, 0.0f });
        monke->set_position({
            (float) (x - half_size) + x_offset,
            (float) (y - half_size),
            5.0f,
        });

        monke->animation_name = animation_name;
        monke->material_name = material_name;

        scene->add_entity(entity_name, monke);
    }
}

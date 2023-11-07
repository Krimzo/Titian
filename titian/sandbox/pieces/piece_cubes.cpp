#include "main.h"


sandbox::SandboxPieceCubes::SandboxPieceCubes(TitianEditor* editor, const int size)
    : SandboxPiece(editor)
{
    this->size = size;
}

void sandbox::SandboxPieceCubes::setup_self()
{
    Scene* scene = &editor->game_layer->scene;

    const int half_size = size / 2;
    int cube_counter = 0;

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            const std::string mesh_name = "cube";
            const std::string material_name = kl::format("cube_mat_", cube_counter);
            const std::string entity_name = kl::format("Cube", cube_counter);

            // Mesh
            scene->meshes[mesh_name] = scene->default_meshes->cube;

            // Material
            kl::Object material = new Material();
            material->color = kl::colors::ORANGE;
            material->reflection_factor = cube_counter / static_cast<float>(size * size);
            scene->materials[material_name] = material;

            // Entity
            kl::Object cube = scene->new_entity(false);
            cube->scale = kl::Float3{ 0.45f };
            cube->set_position({
                (float)(x - half_size) + x_offset,
                (float)(y - half_size),
                5.0f,
                });

            cube->mesh_name = mesh_name;
            cube->material_name = material_name;

            scene->add_entity(entity_name, cube);
            cube_counter += 1;
        }
    }
}

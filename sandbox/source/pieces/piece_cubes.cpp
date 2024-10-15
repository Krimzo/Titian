#include "sandbox.h"


titian::SandboxPieceCubes::SandboxPieceCubes(TitianEditor& editor, const int size)
    : SandboxPiece(editor)
{
    this->size = size;
}

void titian::SandboxPieceCubes::setup_self()
{
    kl::GPU& gpu = editor.app_layer.gpu;
    Scene& scene = editor.game_layer.scene();

    const int half_size = size / 2;
    int cube_counter = 0;

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            const String mesh_name = "cube";
            const String animation_name = "cube";
            const String material_name = kl::format("cube_mat_", cube_counter);
            const String entity_name = kl::format("Cube", cube_counter);

            scene.meshes[mesh_name] = scene.default_meshes.cube.clone();
			scene.animations[animation_name] = scene.default_animations.cube.clone();

            Ref material = new Material();
            material->color = kl::colors::ORANGE;
            material->reflectivity_factor = cube_counter / float(size * size - 1);
            scene.materials[material_name] = material;

            Ref cube = new Entity();
            cube->set_position({
                (float)(x - half_size) + x_offset,
                (float)(y - half_size),
                5.0f,
            });
            cube->animation_name = animation_name;
            cube->material_name = material_name;

            scene.add_entity(entity_name, cube);
            cube_counter += 1;
        }
    }
}

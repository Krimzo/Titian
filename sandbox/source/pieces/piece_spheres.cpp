#include "sandbox.h"


titian::SandboxPieceSpheres::SandboxPieceSpheres(TitianEditor* editor, const int size)
    : SandboxPiece(editor)
{
    this->size = size;
}

void titian::SandboxPieceSpheres::setup_self()
{
    Scene* scene = &editor->game_layer.scene;
    kl::GPU* gpu = &editor->app_layer.gpu;

    const int half_size = size / 2;
    int sphere_counter = 0;

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            const String mesh_name = "sphere";
            const String animation_name = "sphere";
            const String counter_id = kl::format(std::setw(3), std::setfill('0'), sphere_counter);
            const String material_name = kl::format("sphere_mat_", counter_id);
            const String entity_name = kl::format("Sphere", counter_id);

            scene->meshes[mesh_name] = scene->default_meshes->sphere;
			scene->animations[animation_name] = scene->default_animations->sphere;

            Ref material = new Material();
            material->color = Float4{ 1.0f };
            material->reflection_factor = (float) sphere_counter / (size * size);
            material->refraction_factor = (float) (size * size - sphere_counter) / (size * size);
            material->refraction_index = 1.0f / 1.52f;
            scene->materials[material_name] = material;

            Ref sphere = scene->new_entity(false);
            sphere->set_position({ (x - half_size) * 2.25f + x_offset, (y - half_size) * 2.25f, 5.0f });
            sphere->animation_name = animation_name;
            sphere->material_name = material_name;
            scene->add_entity(entity_name, sphere);
            sphere_counter += 1;
        }
    }
}

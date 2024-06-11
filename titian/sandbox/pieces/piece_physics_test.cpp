#include "main.h"


sandbox::SandboxPiecePhysicsTest::SandboxPiecePhysicsTest(TitianEditor* editor, const int size)
    : SandboxPiece(editor)
{
    this->size = size;
}

void sandbox::SandboxPiecePhysicsTest::setup_self()
{
    Scene* scene = &editor->game_layer.scene;
    setup_platform(scene);
    setup_objects(scene);
}

void sandbox::SandboxPiecePhysicsTest::setup_platform(Scene* scene)
{
    const std::string mesh_name = "cube";
    const std::string material_name = "white";
    const std::string entity_name = "PhysicsTestPlatform";

    // Mesh
    scene->meshes[mesh_name] = scene->default_meshes->cube;

    // Material
    scene->materials[material_name] = scene->default_materials->white;

    // Entity
    kl::Object platform = scene->new_entity(false);

    const kl::Float3 scale = { 15.0f, 0.1f, 15.0f };
    platform->scale = scale;

    platform->set_rotation({ 1.0f, 0.0f, 0.0f });
    platform->set_position({ 0.0f, -7.0f, -25.0f });
    platform->set_collider(scene->new_box_collider(scale));

    platform->mesh_name = mesh_name;
    platform->material_name = material_name;

    scene->add_entity(entity_name, platform);
}

void sandbox::SandboxPiecePhysicsTest::setup_objects(Scene* scene)
{
    const int half_size = size / 2;
    const kl::Float3 scale = { 0.5f, 0.5f, 0.5f };
    int box_counter = 0;

    const std::string color_map = "dogo";
    const std::string normal_map = "concrete_normal";

    kl::Object<Texture> color_map_tex;
    kl::Object<Texture> normal_map_tex;

    auto create_texture = [&](kl::Object<Texture>& texture, const char* filename)
    {
        texture = new Texture(&editor->app_layer.gpu);
        texture->data_buffer.load_from_file(filename);
        texture->reload_as_2D(false, false);
        texture->create_shader_view(nullptr);
        kl::assert(texture->shader_view, "Failed to init texture: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_texture(color_map_tex, "preview/textures/dogo.png"); });
    queue.add_task([&] { create_texture(normal_map_tex, "preview/textures/concrete_normal.png"); });
    queue.finalize();

    scene->textures[color_map] = color_map_tex;
    scene->textures[normal_map] = normal_map_tex;

    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            const std::string mesh_name = "cube";
            const std::string counter_id = kl::format(std::setw(3), std::setfill('0'), box_counter);
            const std::string material_name = kl::format("box_mat_", counter_id);
            const std::string entity_name = kl::format("Box", counter_id);

            // Mesh
            scene->meshes[mesh_name] = scene->default_meshes->cube;

            // Material
            kl::Object material = new Material();
            material->texture_blend = 0.5f;
            material->color = kl::random::gen_color();
            material->color_map_name = color_map;
            material->normal_map_name = normal_map;
            scene->materials[material_name] = material;

            // Entity
            kl::Object box = scene->new_entity(true);
            box->scale = scale;
            box->set_rotation(kl::random::gen_float3(360.0f));
            box->set_position({ (half_size - x) * 2.25f, 15.0f, -25.0f + (half_size - z) * 2.25f });
            box->set_collider(scene->new_box_collider(scale));

            box->set_mass(2.5f);
            box->set_gravity(true);

            box->mesh_name = mesh_name;
            box->material_name = material_name;

            scene->add_entity(entity_name, box);
            box_counter += 1;
        }
    }
}

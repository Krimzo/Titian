#include "sandbox.h"


titian::SandboxPiecePhysicsTest::SandboxPiecePhysicsTest(TitianEditor* editor, const int size)
    : SandboxPiece(editor)
{
    this->size = size;
}

void titian::SandboxPiecePhysicsTest::setup_self()
{
    Scene* scene = &editor->game_layer.scene;
    setup_platform(scene);
    setup_objects(scene);
}

void titian::SandboxPiecePhysicsTest::setup_platform(Scene* scene)
{
    const String mesh_name = "cube";
    const String animation_name = "cube";
    const String material_name = "white";
    const String entity_name = "PhysicsTestPlatform";

    scene->meshes[mesh_name] = scene->default_meshes->cube;
    scene->animations[animation_name] = scene->default_animations->cube;
    scene->materials[material_name] = scene->default_materials->white;

    Ref platform = scene->new_entity();

    const Float3 scale = { 15.0f, 0.1f, 15.0f };
    platform->set_scale(scale);

    platform->set_rotation({ 1.0f, 0.0f, 0.0f });
    platform->set_position({ 0.0f, -7.0f, -25.0f });
    platform->set_collider(scene->new_box_collider(scale));

    platform->animation_name = animation_name;
    platform->material_name = material_name;

    scene->add_entity(entity_name, platform);
}

void titian::SandboxPiecePhysicsTest::setup_objects(Scene* scene)
{
    const int half_size = size / 2;
    const Float3 scale = { 0.5f, 0.5f, 0.5f };
    int box_counter = 0;

    const String color_map = "dogo";
    const String normal_map = "concrete_normal";

    Ref<Texture> color_map_tex;
    Ref<Texture> normal_map_tex;

    auto create_texture = [&](Ref<Texture>& texture, const char* filename)
    {
        texture = new Texture(&editor->app_layer.gpu);
        texture->data_buffer.load_from_file(filename);
        texture->reload_as_2D();
        texture->create_shader_view(nullptr);
        kl::assert(texture->shader_view, "Failed to init texture: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_texture(color_map_tex, "package/textures/dogo.png"); });
    queue.add_task([&] { create_texture(normal_map_tex, "package/textures/concrete_normal.png"); });
    queue.finalize();

    scene->textures[color_map] = color_map_tex;
    scene->textures[normal_map] = normal_map_tex;

    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            const String mesh_name = "cube";
            const String animation_name = "cube";
            const String counter_id = kl::format(std::setw(3), std::setfill('0'), box_counter);
            const String material_name = kl::format("box_mat_", counter_id);
            const String entity_name = kl::format("Box", counter_id);

            scene->meshes[mesh_name] = scene->default_meshes->cube;
			scene->animations[animation_name] = scene->default_animations->cube;

            Ref material = new Material();
            material->texture_blend = 0.5f;
            material->color = kl::random::gen_color();
            material->color_map_name = color_map;
            material->normal_map_name = normal_map;
            scene->materials[material_name] = material;

            Ref box = scene->new_entity();
            box->set_dynamic(true);

            box->set_scale(scale);
            box->set_rotation(kl::random::gen_float3(360.0f));
            box->set_position({ (half_size - x) * 2.25f, 15.0f + (z - size / 2) * 1.5f, -25.0f + (half_size - z) * 2.25f});
            box->set_collider(scene->new_box_collider(scale));

            box->set_mass(2.5f);
            box->set_gravity(true);

            box->animation_name = animation_name;
            box->material_name = material_name;

            scene->add_entity(entity_name, box);
            box_counter += 1;
        }
    }
}

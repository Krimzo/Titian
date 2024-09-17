#include "sandbox.h"


titian::SandboxPiecePhysicsTest::SandboxPiecePhysicsTest(TitianEditor* editor, const int size)
    : SandboxPiece(editor), size(size)
{}

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

    Ref platform = new Entity();

    const Float3 scale = { 30.0f, 0.2f, 30.0f };
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
    int box_counter = 0;

    const String color_name = "dogo";
    const String normal_name = "concrete_normal";

    Ref<Texture> color_texture;
    Ref<Texture> normal_texture;

    const auto create_texture = [&](Ref<Texture>& texture, const char* filename)
    {
        texture = new Texture();
        texture->data_buffer.load_from_file(filename);
        texture->reload_as_2D();
        texture->create_shader_view(nullptr);
        kl::assert(texture->shader_view, "Failed to init texture: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_texture(color_texture, "package/textures/dogo.png"); });
    queue.add_task([&] { create_texture(normal_texture, "package/textures/concrete_normal.png"); });
    queue.finalize();

    scene->textures[color_name] = color_texture;
    scene->textures[normal_name] = normal_texture;

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
            material->color_texture_name = color_name;
            material->normal_texture_name = normal_name;
            scene->materials[material_name] = material;

            Ref box = new Entity();
            box->set_dynamic(true);

            box->set_rotation(kl::random::gen_float3(360.0f));
            box->set_position({ (half_size - x) * 2.25f, 15.0f + (z - size / 2) * 1.5f, -25.0f + (half_size - z) * 2.25f});
            box->set_collider(scene->new_box_collider(Float3{ 1.0f }));

            box->set_mass(2.5f);
            box->set_gravity(true);

            box->animation_name = animation_name;
            box->material_name = material_name;

            scene->add_entity(entity_name, box);
            box_counter += 1;
        }
    }
}

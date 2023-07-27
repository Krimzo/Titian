#include "serialization/serializer.h"


serializer::serializer(const std::string& path)
    : path(path)
{}

bool serializer::write_scene(const kl::object<kl::scene>& scene)
{
    kl::file file = { path };
    if (!file) return false;

    // Serialsation version
    file.write<uint32_t>(SERIAL_VERSION);

    // Mesh data
    file.write<uint64_t>(scene->meshes.size());
    for (const auto& [mesh_name, mesh] : scene->meshes) {
        // Name
        file.write<uint64_t>(mesh_name.size());
        file.write<char>(mesh_name.data(), mesh_name.size());

        // Vertices
        file.write<uint64_t>(mesh->data_buffer.size());
        file.write<kl::vertex>(mesh->data_buffer.data(), mesh->data_buffer.size());
    }

    // Texture data
    file.write<uint64_t>(scene->textures.size());
    for (const auto& [texture_name, texture] : scene->textures) {
        // Name
        file.write<uint64_t>(texture_name.size());
        file.write<char>(texture_name.data(), texture_name.size());

        // Pixels
        file.write<kl::int2>(texture->data_buffer.size());
        file.write<kl::color>(texture->data_buffer, texture->data_buffer.pixel_count());

        // Cube?
        file.write<bool>(texture->is_cube);
    }

    // Material data
    file.write<uint64_t>(scene->materials.size());
    for (const auto& [material_name, material] : scene->materials) {
        // Name
        file.write<uint64_t>(material_name.size());
        file.write<char>(material_name.data(), material_name.size());

        // Blend
        file.write<float>(material->texture_blend);
        file.write<float>(material->reflection_factor);
        file.write<float>(material->refraction_factor);
        file.write<float>(material->refraction_index);

        // Color
        file.write<kl::float4>(material->color);

        // Bound textures
        std::string color_map_name = {};
        std::string normal_map_name = {};
        std::string roughness_map_name = {};

        for (const auto& [texture_name, texture] : scene->textures) {
            if (texture == material->color_map) {
                color_map_name = texture_name;
            }
            if (texture == material->normal_map) {
                normal_map_name = texture_name;
            }
            if (texture == material->roughness_map) {
                roughness_map_name = texture_name;
            }
        }

        file.write<uint64_t>(color_map_name.size());
        file.write<char>(color_map_name.data(), color_map_name.size());
        file.write<uint64_t>(normal_map_name.size());
        file.write<char>(normal_map_name.data(), normal_map_name.size());
        file.write<uint64_t>(roughness_map_name.size());
        file.write<char>(roughness_map_name.data(), roughness_map_name.size());
    }

    // Entity data
    file.write<uint64_t>(scene->entity_count());
    for (const auto& [entity_name, entity] : *scene) {
        // Name
        file.write<uint64_t>(entity_name.size());
        file.write<char>(entity_name.data(), entity_name.size());

        // Scale
        file.write<kl::float3>(entity->render_scale);

        // Bound mesh/material
        std::string ent_mesh_name = {};
        std::string ent_material_name = {};

        for (const auto& [mesh_name, mesh] : scene->meshes) {
            if (mesh == entity->mesh) {
                ent_mesh_name = mesh_name;
                break;
            }
        }
        for (const auto& [material_name, material] : scene->materials) {
            if (material == entity->material) {
                ent_material_name = material_name;
                break;
            }
        }

        file.write<uint64_t>(ent_mesh_name.size());
        file.write<char>(ent_mesh_name.data(), ent_mesh_name.size());
        file.write<uint64_t>(ent_material_name.size());
        file.write<char>(ent_material_name.data(), ent_material_name.size());

        // Geometry
        file.write<kl::float3>(entity->rotation());
        file.write<kl::float3>(entity->position());

        // Physics
        file.write<bool>(entity->is_dynamic());
        file.write<bool>(entity->has_gravity());
        file.write<float>(entity->mass());
        file.write<kl::float3>(entity->velocity());
        file.write<kl::float3>(entity->angular());

        // Collider
        const kl::object<kl::collider>& collider = entity->collider();
        file.write<bool>((bool) collider);
        if (collider) {
            // Type
            file.write<PxGeometryType::Enum>(collider->type());

            // Geometry
            file.write<kl::float3>(collider->rotation());
            file.write<kl::float3>(collider->offset());

            // Material
            file.write<float>(collider->static_friction());
            file.write<float>(collider->dynamic_friction());
            file.write<float>(collider->restitution());
        }
    }

    // Camera
    const kl::object<kl::camera>& camera = scene->camera;
    file.write<bool>((bool) camera);
    if (camera) {
        // Direction
        file.write<kl::float3>(camera->forward());
        file.write<kl::float3>(camera->up());

        // Misc Info
        file.write<kl::float3>(camera->origin);
        file.write<float>(camera->aspect_ratio);
        file.write<float>(camera->field_of_view);
        file.write<float>(camera->near_plane);
        file.write<float>(camera->far_plane);
        file.write<float>(camera->sensitivity);
        file.write<float>(camera->speed);
        file.write<kl::color>(camera->background);

        // Skybox
        std::string skybox_name = {};
        for (const auto& [texture_name, texture] : scene->textures) {
            if (texture == camera->skybox) {
                skybox_name = texture_name;
                break;
            }
        }
        file.write<uint64_t>(skybox_name.size());
        file.write<char>(skybox_name.data(), skybox_name.size());
    }

    // Selected Entity
    std::string selected_entity_name = {};
    for (const auto& [entity_name, entity] : *scene) {
        if (entity == scene->selected_entity) {
            selected_entity_name = entity_name;
            break;
        }
    }
    file.write<uint64_t>(selected_entity_name.size());
    file.write<char>(selected_entity_name.data(), selected_entity_name.size());

    // Ambient Light
    const kl::object<kl::ambient_light>& ambient_light = scene->ambient_light;
    file.write<bool>((bool) ambient_light);
    if (ambient_light) {
        file.write<kl::ambient_light>(*ambient_light);
    }

    // Directional Light
    const kl::object<kl::directional_light>& directional_light = scene->directional_light;
    file.write<bool>((bool) directional_light);
    if (directional_light) {
        file.write<kl::float3>(directional_light->direction());
        file.write<UINT>(directional_light->map_resolution);
        file.write<float>(directional_light->point_size);
    }

    // Finalize
    file.close();
    return true;
}

std::pair<uint32_t, kl::object<kl::scene>> serializer::read_scene(kl::gpu* gpu)
{
    kl::file file = { path, false };
    if (!file) return { 1, nullptr };

    // Version check
    uint32_t serial_version = 0;
    file.read<uint32_t>(serial_version);
    if (serial_version != SERIAL_VERSION) {
        return { serial_version, nullptr };
    }
    kl::object scene = new kl::scene();

    // Mesh data
    uint64_t mesh_count = 0;
    file.read<uint64_t>(mesh_count);
    for (uint64_t i = 0; i < mesh_count; i++) {
        // Create mesh
        kl::object<kl::mesh> mesh = new kl::mesh(gpu, &scene);
        std::string mesh_name = {};

        // Name
        uint64_t mesh_name_size = 0;
        file.read<uint64_t>(mesh_name_size);
        mesh_name.resize(mesh_name_size);
        file.read<char>(mesh_name.data(), mesh_name_size);

        // Vertices
        uint64_t vertex_count = 0;
        file.read<uint64_t>(vertex_count);
        mesh->data_buffer.resize(vertex_count);
        file.read<kl::vertex>(mesh->data_buffer.data(), vertex_count);

        // Store mesh
        mesh->reload();
        scene->meshes[mesh_name] = mesh;
    }

    // Texture data
    uint64_t texture_count = 0;
    file.read<uint64_t>(texture_count);
    for (uint64_t i = 0; i < texture_count; i++) {
        // Create texture
        kl::object<kl::texture> texture = new kl::texture(gpu);
        std::string texture_name = {};

        // Name
        uint64_t texture_name_size = 0;
        file.read<uint64_t>(texture_name_size);
        texture_name.resize(texture_name_size);
        file.read<char>(texture_name.data(), texture_name_size);

        // Pixels
        kl::int2 texture_size = {};
        file.read<kl::int2>(texture_size);
        texture->data_buffer.resize(texture_size);
        file.read<kl::color>(texture->data_buffer, texture->data_buffer.pixel_count());

        // Cube?
        file.read<bool>(texture->is_cube);

        // Store texture
        if (texture->is_cube) {
            texture->reload_as_cube();
        }
        else {
            texture->reload();
        }
        texture->create_shader_view();
        scene->textures[texture_name] = texture;
    }

    //// Material data
    //file.write<uint64_t>(scene->materials.size());
    //for (const auto& [material_name, material] : scene->materials) {
    //    // Name
    //    file.write<uint64_t>(material_name.size());
    //    file.write<char>(material_name.data(), material_name.size());
    //
    //    // Blend
    //    file.write<float>(material->texture_blend);
    //    file.write<float>(material->reflection_factor);
    //    file.write<float>(material->refraction_factor);
    //    file.write<float>(material->refraction_index);
    //
    //    // Color
    //    file.write<kl::float4>(material->color);
    //
    //    // Bound textures
    //    std::string color_map_name = {};
    //    std::string normal_map_name = {};
    //    std::string roughness_map_name = {};
    //
    //    for (const auto& [texture_name, texture] : scene->textures) {
    //        if (texture == material->color_map) {
    //            color_map_name = texture_name;
    //        }
    //        if (texture == material->normal_map) {
    //            normal_map_name = texture_name;
    //        }
    //        if (texture == material->roughness_map) {
    //            roughness_map_name = texture_name;
    //        }
    //    }
    //
    //    file.write<uint64_t>(color_map_name.size());
    //    file.write<char>(color_map_name.data(), color_map_name.size());
    //    file.write<uint64_t>(normal_map_name.size());
    //    file.write<char>(normal_map_name.data(), normal_map_name.size());
    //    file.write<uint64_t>(roughness_map_name.size());
    //    file.write<char>(roughness_map_name.data(), roughness_map_name.size());
    //}
    //
    //// Entity data
    //file.write<uint64_t>(scene->entity_count());
    //for (const auto& [entity_name, entity] : *scene) {
    //    // Name
    //    file.write<uint64_t>(entity_name.size());
    //    file.write<char>(entity_name.data(), entity_name.size());
    //
    //    // Scale
    //    file.write<kl::float3>(entity->render_scale);
    //
    //    // Bound mesh/material
    //    std::string ent_mesh_name = {};
    //    std::string ent_material_name = {};
    //
    //    for (const auto& [mesh_name, mesh] : scene->meshes) {
    //        if (mesh == entity->mesh) {
    //            ent_mesh_name = mesh_name;
    //            break;
    //        }
    //    }
    //    for (const auto& [material_name, material] : scene->materials) {
    //        if (material == entity->material) {
    //            ent_material_name = material_name;
    //            break;
    //        }
    //    }
    //
    //    file.write<uint64_t>(ent_mesh_name.size());
    //    file.write<char>(ent_mesh_name.data(), ent_mesh_name.size());
    //    file.write<uint64_t>(ent_material_name.size());
    //    file.write<char>(ent_material_name.data(), ent_material_name.size());
    //
    //    // Geometry
    //    file.write<kl::float3>(entity->rotation());
    //    file.write<kl::float3>(entity->position());
    //
    //    // Physics
    //    file.write<bool>(entity->is_dynamic());
    //    file.write<bool>(entity->has_gravity());
    //    file.write<float>(entity->mass());
    //    file.write<kl::float3>(entity->velocity());
    //    file.write<kl::float3>(entity->angular());
    //
    //    // Collider
    //    const kl::object<kl::collider>& collider = entity->collider();
    //    file.write<bool>((bool) collider);
    //    if (collider) {
    //        // Type
    //        file.write<PxGeometryType::Enum>(collider->type());
    //
    //        // Geometry
    //        file.write<kl::float3>(collider->rotation());
    //        file.write<kl::float3>(collider->offset());
    //
    //        // Material
    //        file.write<float>(collider->static_friction());
    //        file.write<float>(collider->dynamic_friction());
    //        file.write<float>(collider->restitution());
    //    }
    //}
    //
    //// Camera
    //const kl::object<kl::camera>& camera = scene->camera;
    //file.write<bool>((bool) camera);
    //if (camera) {
    //    // Direction
    //    file.write<kl::float3>(camera->forward());
    //    file.write<kl::float3>(camera->up());
    //
    //    // Misc Info
    //    file.write<kl::float3>(camera->origin);
    //    file.write<float>(camera->aspect_ratio);
    //    file.write<float>(camera->field_of_view);
    //    file.write<float>(camera->near_plane);
    //    file.write<float>(camera->far_plane);
    //    file.write<float>(camera->sensitivity);
    //    file.write<float>(camera->speed);
    //    file.write<kl::color>(camera->background);
    //
    //    // Skybox
    //    std::string skybox_name = {};
    //    for (const auto& [texture_name, texture] : scene->textures) {
    //        if (texture == camera->skybox) {
    //            skybox_name = texture_name;
    //            break;
    //        }
    //    }
    //    file.write<uint64_t>(skybox_name.size());
    //    file.write<char>(skybox_name.data(), skybox_name.size());
    //}
    //
    //// Selected Entity
    //std::string selected_entity_name = {};
    //for (const auto& [entity_name, entity] : *scene) {
    //    if (entity == scene->selected_entity) {
    //        selected_entity_name = entity_name;
    //        break;
    //    }
    //}
    //file.write<uint64_t>(selected_entity_name.size());
    //file.write<char>(selected_entity_name.data(), selected_entity_name.size());
    //
    //// Ambient Light
    //const kl::object<kl::ambient_light>& ambient_light = scene->ambient_light;
    //file.write<bool>((bool) ambient_light);
    //if (ambient_light) {
    //    file.write<kl::ambient_light>(*ambient_light);
    //}
    //
    //// Directional Light
    //const kl::object<kl::directional_light>& directional_light = scene->directional_light;
    //file.write<bool>((bool) directional_light);
    //if (directional_light) {
    //    file.write<kl::float3>(directional_light->direction());
    //    file.write<UINT>(directional_light->map_resolution);
    //    file.write<float>(directional_light->point_size);
    //}

    // Finalize
    file.close();
    return { 0, scene };
}

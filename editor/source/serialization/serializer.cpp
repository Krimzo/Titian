#include "editor.h"


serializer::serializer(const std::string& path)
    : path(path), m_file(path, true)
{}

serializer::~serializer()
{
    m_file.close();
}

bool serializer::write_scene(const kl::object<kl::scene>& scene)
{
    // File check
    if (!m_file) return false;

    // Serialsation version
    m_file.write<uint32_t>(serialization::VERSION);

    // Mesh data
    m_file.write<uint64_t>(scene->meshes.size());
    for (const auto& [mesh_name, mesh] : scene->meshes) {
        // Name
        write_string(mesh_name);

        // Vertices
        m_file.write<uint64_t>(mesh->data_buffer.size());
        m_file.write<kl::vertex>(mesh->data_buffer.data(), mesh->data_buffer.size());
    }

    // Texture data
    m_file.write<uint64_t>(scene->textures.size());
    for (const auto& [texture_name, texture] : scene->textures) {
        // Name
        write_string(texture_name);

        // Pixels
        m_file.write<kl::int2>(texture->data_buffer.size());
        m_file.write<kl::color>(texture->data_buffer, texture->data_buffer.pixel_count());

        // Cube?
        m_file.write<bool>(texture->is_cube);
    }

    // Material data
    m_file.write<uint64_t>(scene->materials.size());
    for (const auto& [material_name, material] : scene->materials) {
        // Name
        write_string(material_name);

        // Blend
        m_file.write<float>(material->texture_blend);
        m_file.write<float>(material->reflection_factor);
        m_file.write<float>(material->refraction_factor);
        m_file.write<float>(material->refraction_index);

        // Color
        m_file.write<kl::float4>(material->color);

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

        write_string(color_map_name);
        write_string(normal_map_name);
        write_string(roughness_map_name);
    }

    // Entity data
    m_file.write<uint64_t>(scene->entity_count());
    for (const auto& [entity_name, entity] : *scene) {
        // Name/Dynamic
        write_string(entity_name);
        m_file.write<bool>(entity->is_dynamic());

        // Render scale
        m_file.write<kl::float3>(entity->render_scale);

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

        write_string(ent_mesh_name);
        write_string(ent_material_name);

        // Geometry
        m_file.write<kl::float3>(entity->rotation());
        m_file.write<kl::float3>(entity->position());

        // Physics
        m_file.write<bool>(entity->has_gravity());
        m_file.write<float>(entity->mass());
        m_file.write<kl::float3>(entity->velocity());
        m_file.write<kl::float3>(entity->angular());

        // Collider
        kl::object<kl::collider> collider = entity->collider();
        m_file.write<bool>((bool) collider);
        if (collider) {
            // Geomtry data
            const PxGeometryHolder& geometry_holder = collider->shape()->getGeometry();
            write_collider_geometry(scene, geometry_holder);

            // Geometry info
            m_file.write<kl::float3>(collider->rotation());
            m_file.write<kl::float3>(collider->offset());

            // Physics material
            m_file.write<float>(collider->static_friction());
            m_file.write<float>(collider->dynamic_friction());
            m_file.write<float>(collider->restitution());
        }
    }

    // Camera
    const kl::object<kl::camera>& camera = scene->camera;
    m_file.write<bool>((bool) camera);
    if (camera) {
        // Direction
        m_file.write<kl::float3>(camera->forward());
        m_file.write<kl::float3>(camera->up());

        // Misc Info
        m_file.write<kl::float3>(camera->origin);
        m_file.write<float>(camera->aspect_ratio);
        m_file.write<float>(camera->field_of_view);
        m_file.write<float>(camera->near_plane);
        m_file.write<float>(camera->far_plane);
        m_file.write<float>(camera->sensitivity);
        m_file.write<float>(camera->speed);
        m_file.write<kl::color>(camera->background);

        // Skybox
        std::string skybox_name = {};
        for (const auto& [texture_name, texture] : scene->textures) {
            if (texture == camera->skybox) {
                skybox_name = texture_name;
                break;
            }
        }
        write_string(skybox_name);
    }

    // Selected Entity
    std::string selected_entity_name = {};
    for (const auto& [entity_name, entity] : *scene) {
        if (entity == scene->selected_entity) {
            selected_entity_name = entity_name;
            break;
        }
    }
    write_string(selected_entity_name);

    // Ambient Light
    const kl::object<kl::ambient_light>& ambient_light = scene->ambient_light;
    m_file.write<bool>((bool) ambient_light);
    if (ambient_light) {
        m_file.write<kl::ambient_light>(*ambient_light);
    }

    // Directional Light
    const kl::object<kl::directional_light>& directional_light = scene->directional_light;
    m_file.write<bool>((bool) directional_light);
    if (directional_light) {
        m_file.write<UINT>(directional_light->map_resolution);
        m_file.write<kl::float3>(directional_light->direction());
        m_file.write<float>(directional_light->point_size);
    }

    // Finalize
    return true;
}

void serializer::write_string(const std::string& data)
{
    m_file.write<uint64_t>(data.size());
    m_file.write<char>(data.data(), data.size());
}

void serializer::write_collider_geometry(const kl::object<kl::scene>& scene, const PxGeometryHolder& geometry_holder)
{
    // Type
    const PxGeometryType::Enum type = geometry_holder.getType();
    m_file.write<PxGeometryType::Enum>(type);

    // Data
    switch (type) {
    case PxGeometryType::Enum::eBOX:
    {
        const PxBoxGeometry& box_geometry = geometry_holder.box();
        m_file.write<PxVec3>(box_geometry.halfExtents);
    }
    break;

    case PxGeometryType::Enum::eSPHERE:
    {
        const PxSphereGeometry& sphere_geometry = geometry_holder.sphere();
        m_file.write<float>(sphere_geometry.radius);
    }
    break;

    case PxGeometryType::Enum::eCAPSULE:
    {
        const PxCapsuleGeometry& capsule_geometry = geometry_holder.capsule();
        m_file.write<float>(capsule_geometry.radius);
        m_file.write<float>(capsule_geometry.halfHeight);
    }
    break;

    case PxGeometryType::Enum::eTRIANGLEMESH:
    {
        const PxTriangleMeshGeometry& triangle_mesh_geometry = geometry_holder.triangleMesh();
        m_file.write<PxMeshScale>(triangle_mesh_geometry.scale);

        std::string collider_mesh_name = {};
        for (const auto& [mesh_name, mesh] : scene->meshes) {
            if (mesh->physics_buffer == triangle_mesh_geometry.triangleMesh) {
                collider_mesh_name = mesh_name;
                break;
            }
        }
        write_string(collider_mesh_name);
    }
    break;
    }
}

export module serializer;

export import serialization;

export class Serializer
{
    kl::File m_file = {};

    void write_string(const std::string& data)
    {
        m_file.write<uint64_t>(data.size());
        m_file.write<char>(data.data(), data.size());
    }

    void write_collider_geometry(const kl::Object<kl::Scene>& scene, const physx::PxGeometryHolder& geometry_holder)
    {
        // Type
        const physx::PxGeometryType::Enum type = geometry_holder.getType();
        m_file.write<physx::PxGeometryType::Enum>(type);

        // Data
        switch (type) {
        case physx::PxGeometryType::Enum::eBOX:
        {
            const physx::PxBoxGeometry& box_geometry = geometry_holder.box();
            m_file.write<physx::PxVec3>(box_geometry.halfExtents);
        }
        break;

        case physx::PxGeometryType::Enum::eSPHERE:
        {
            const physx::PxSphereGeometry& sphere_geometry = geometry_holder.sphere();
            m_file.write<float>(sphere_geometry.radius);
        }
        break;

        case physx::PxGeometryType::Enum::eCAPSULE:
        {
            const physx::PxCapsuleGeometry& capsule_geometry = geometry_holder.capsule();
            m_file.write<float>(capsule_geometry.radius);
            m_file.write<float>(capsule_geometry.halfHeight);
        }
        break;

        case physx::PxGeometryType::Enum::eTRIANGLEMESH:
        {
            const physx::PxTriangleMeshGeometry& triangle_mesh_geometry = geometry_holder.triangleMesh();
            m_file.write<physx::PxMeshScale>(triangle_mesh_geometry.scale);

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

public:
    const std::string path = {};
    
    Serializer(const std::string& path)
        : m_file(path, true), path(path)
    {}

    virtual ~Serializer()
    {
        m_file.close();
    }

    bool write_scene(const kl::Object<kl::Scene>& scene)
    {
        // File check
        if (!m_file) return false;

        // Serialization version
        m_file.write<uint32_t>(serialization::VERSION);

        // Mesh data
        m_file.write<uint64_t>(scene->meshes.size());
        for (const auto& [mesh_name, mesh] : scene->meshes) {
            // Name
            write_string(mesh_name);

            // Vertices
            m_file.write<uint64_t>(mesh->data_buffer.size());
            m_file.write<kl::Vertex>(mesh->data_buffer.data(), mesh->data_buffer.size());
        }

        // Texture data
        m_file.write<uint64_t>(scene->textures.size());
        for (const auto& [texture_name, texture] : scene->textures) {
            // Name
            write_string(texture_name);

            // Pixels
            m_file.write<kl::Int2>(texture->data_buffer.size());
            m_file.write<kl::Color>(texture->data_buffer, texture->data_buffer.pixel_count());

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
            m_file.write<kl::Float4>(material->color);

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
            m_file.write<kl::Float3>(entity->render_scale);

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
            m_file.write<kl::Float3>(entity->rotation());
            m_file.write<kl::Float3>(entity->position());

            // Physics
            m_file.write<bool>(entity->has_gravity());
            m_file.write<float>(entity->mass());
            m_file.write<kl::Float3>(entity->velocity());
            m_file.write<kl::Float3>(entity->angular());

            // Collider
            kl::Object<kl::Collider> collider = entity->collider();
            m_file.write<bool>((bool) collider);
            if (collider) {
                // Geometry data
                const physx::PxGeometryHolder& geometry_holder = collider->shape()->getGeometry();
                write_collider_geometry(scene, geometry_holder);

                // Geometry info
                m_file.write<kl::Float3>(collider->rotation());
                m_file.write<kl::Float3>(collider->offset());

                // Physics material
                m_file.write<float>(collider->static_friction());
                m_file.write<float>(collider->dynamic_friction());
                m_file.write<float>(collider->restitution());
            }
        }

        // Camera
        const kl::Object<kl::Camera>& camera = scene->camera;
        m_file.write<bool>((bool) camera);
        if (camera) {
            // Direction
            m_file.write<kl::Float3>(camera->forward());
            m_file.write<kl::Float3>(camera->up());

            // Misc Info
            m_file.write<kl::Float3>(camera->origin);
            m_file.write<float>(camera->aspect_ratio);
            m_file.write<float>(camera->field_of_view);
            m_file.write<float>(camera->near_plane);
            m_file.write<float>(camera->far_plane);
            m_file.write<float>(camera->sensitivity);
            m_file.write<float>(camera->speed);
            m_file.write<kl::Color>(camera->background);

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
        const kl::Object<kl::AmbientLight>& ambient_light = scene->ambient_light;
        m_file.write<bool>((bool) ambient_light);
        if (ambient_light) {
            m_file.write<kl::AmbientLight>(*ambient_light);
        }

        // Directional Light
        const kl::Object<kl::DirectionalLight>& directional_light = scene->directional_light;
        m_file.write<bool>(directional_light);
        if (directional_light) {
            m_file.write<UINT>(directional_light->map_resolution);
            m_file.write<kl::Float3>(directional_light->direction());
            m_file.write<float>(directional_light->point_size);
        }

        // Finalize
        return true;
    }
};

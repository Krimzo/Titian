#include "serialization/deserializer.h"


deserializer::deserializer(const std::string& path)
    : path(path), m_file(path, false)
{}

deserializer::~deserializer()
{
    m_file.close();
}

std::pair<uint32_t, kl::object<kl::scene>> deserializer::read_scene(kl::object<kl::gpu>& gpu)
{
    // File check
    if (!m_file) return { 1, nullptr };

    // Version check
    const uint32_t serial_version = m_file.read<uint32_t>();
    if (serial_version != serialization::VERSION) {
        return { serial_version, nullptr };
    }
    kl::object scene = new kl::scene();

    // Mesh data
    const uint64_t mesh_count = m_file.read<uint64_t>();
    for (uint64_t i = 0; i < mesh_count; i++) {
        // Create mesh
        const std::string mesh_name = read_string();
        kl::object<kl::mesh> mesh = new kl::mesh(&gpu, &scene);

        // Vertices
        const uint64_t vertex_count = m_file.read<uint64_t>();
        mesh->data_buffer.resize(vertex_count);
        m_file.read<kl::vertex>(mesh->data_buffer.data(), vertex_count);

        // Store mesh
        mesh->reload();
        scene->meshes[mesh_name] = mesh;
    }

    // Texture data
    const uint64_t texture_count = m_file.read<uint64_t>();
    for (uint64_t i = 0; i < texture_count; i++) {
        // Create texture
        const std::string texture_name = read_string();
        kl::object<kl::texture> texture = new kl::texture(&gpu);

        // Pixels
        const kl::int2 texture_size = m_file.read<kl::int2>();
        texture->data_buffer.resize(texture_size);
        m_file.read<kl::color>(texture->data_buffer, texture->data_buffer.pixel_count());

        // Cube?
        texture->is_cube = m_file.read<bool>();

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

    // Material data
    const uint64_t material_count = m_file.read<uint64_t>();
    for (uint64_t i = 0; i < material_count; i++) {
        // Create material
        const std::string material_name = read_string();
        kl::object<kl::material> material = new kl::material();
        
        // Blend
        material->texture_blend = m_file.read<float>();
        material->reflection_factor = m_file.read<float>();
        material->refraction_factor = m_file.read<float>();
        material->refraction_index = m_file.read<float>();

        // Color
        material->color = m_file.read<kl::float4>();

        // Bound textures
        const std::string color_map_name = read_string();
        material->color_map = scene->textures.contains(color_map_name) ? scene->textures.at(color_map_name) : nullptr;
        const std::string normal_map_name = read_string();
        material->normal_map = scene->textures.contains(normal_map_name) ? scene->textures.at(normal_map_name) : nullptr;
        const std::string roughness_map_name = read_string();
        material->roughness_map = scene->textures.contains(roughness_map_name) ? scene->textures.at(roughness_map_name) : nullptr;

        // Store material
        scene->materials[material_name] = material;
    }
    
    // Entity data
    const uint64_t entity_count = m_file.read<uint64_t>();
    for (uint64_t i = 0; i < entity_count; i++) {
        // Create entity
        const std::string entity_name = read_string();
        const bool is_dynamic = m_file.read<bool>();
        kl::object<kl::entity> entity = new kl::entity(scene->physics(), is_dynamic);

        // Render scale
        entity->render_scale = m_file.read<kl::float3>();
        
        // Bound mesh/material
        const std::string mesh_name = read_string();
        if (scene->meshes.contains(mesh_name)) {
            entity->mesh = scene->meshes.at(mesh_name);
        }
        const std::string material_name = read_string();
        if (scene->materials.contains(material_name)) {
            entity->material = scene->materials.at(material_name);
        }
    
        // Geometry
        entity->set_rotation(m_file.read<kl::float3>());
        entity->set_position(m_file.read<kl::float3>());
        
        // Physics
        entity->set_gravity(m_file.read<bool>());
        entity->set_mass(m_file.read<float>());
        entity->set_velocity(m_file.read<kl::float3>());
        entity->set_angular(m_file.read<kl::float3>());
        
        // Collider
        const bool has_collider = m_file.read<bool>();
        if (has_collider) {
            // Create collider
            const PxGeometryHolder geometry_holder = read_collider_geometry(scene);
            kl::object<kl::collider> collider = new kl::collider(scene->physics(), geometry_holder.any());
    
            // Geometry info
            collider->set_rotation(m_file.read<kl::float3>());
            collider->set_offset(m_file.read<kl::float3>());
        
            // Physics material
            collider->set_static_friction(m_file.read<float>());
            collider->set_dynamic_friction(m_file.read<float>());
            collider->set_restitution(m_file.read<float>());
    
            // Set collider
            entity->set_collider(collider);
        }
        
        // Store entity
        scene->add(entity_name, entity);
    }
    
    // Camera
    const bool has_camera = m_file.read<bool>();
    if (has_camera) {
        // Create camera
        kl::object<kl::camera> camera = new kl::camera();

        // Direction
        camera->set_forward(m_file.read<kl::float3>());
        camera->set_up(m_file.read<kl::float3>());
    
        // Misc Info
        camera->origin = m_file.read<kl::float3>();
        camera->aspect_ratio = m_file.read<float>();
        camera->field_of_view = m_file.read<float>();
        camera->near_plane = m_file.read<float>();
        camera->far_plane = m_file.read<float>();
        camera->sensitivity = m_file.read<float>();
        camera->speed = m_file.read<float>();
        camera->background = m_file.read<kl::color>();
    
        // Skybox
        const std::string skybox_name = read_string();
        camera->skybox = scene->textures.contains(skybox_name) ? scene->textures.at(skybox_name) : nullptr;

        // Set camera
        scene->camera = camera;
    }
    
    // Selected Entity
    const std::string selected_entity_name = read_string();
    scene->selected_entity = scene->find_entity(selected_entity_name);
    
    // Ambient Light
    const bool has_ambient_light = m_file.read<bool>();
    if (has_ambient_light) {
        // Create light
        kl::object<kl::ambient_light> light = new kl::ambient_light();

        // Data
        m_file.read<kl::ambient_light>(*light);

        // Set light
        scene->ambient_light = light;
    }
    
    // Directional Light
    const bool has_directional_light = m_file.read<bool>();
    if (has_directional_light) {
        // Create light
        const UINT map_resolution = m_file.read<UINT>();
        kl::object<kl::directional_light> light = new kl::directional_light(&gpu, map_resolution);

        // Data
        light->set_direction(m_file.read<kl::float3>());
        light->point_size = m_file.read<float>();

        // Set light
        scene->directional_light = light;
    }

    // Finalize
    return { 0, scene };
}

std::string deserializer::read_string()
{
    uint64_t string_size = 0;
    m_file.read<uint64_t>(string_size);

    std::string result = {};
    result.resize(string_size);
    m_file.read<char>(result.data(), string_size);
    return result;
}

PxGeometryHolder deserializer::read_collider_geometry(const kl::object<kl::scene>& scene)
{
    // Type
    const PxGeometryType::Enum type = m_file.read<PxGeometryType::Enum>();

    // Data
    switch (type) {
    case PxGeometryType::Enum::eBOX:
    {
        PxBoxGeometry box_geometry = {};
        box_geometry.halfExtents = m_file.read<PxVec3>();
        return { box_geometry };
    }

    case PxGeometryType::Enum::eSPHERE:
    {
        PxSphereGeometry sphere_geometry = {};
        sphere_geometry.radius = m_file.read<float>();
        return { sphere_geometry };
    }

    case PxGeometryType::Enum::eCAPSULE:
    {
        PxCapsuleGeometry capsule_geometry = {};
        capsule_geometry.radius = m_file.read<float>();
        capsule_geometry.halfHeight = m_file.read<float>();
        return { capsule_geometry };
    }

    case PxGeometryType::Enum::eTRIANGLEMESH:
    {
        PxTriangleMeshGeometry triangle_mesh_geometry = {};
        triangle_mesh_geometry.scale = m_file.read<PxMeshScale>();

        const std::string collider_mesh_name = read_string();
        for (const auto& [mesh_name, mesh] : scene->meshes) {
            if (mesh_name == collider_mesh_name) {
                triangle_mesh_geometry.triangleMesh = mesh->physics_buffer;
                break;
            }
        }
        return { triangle_mesh_geometry };
    }
    }
    return {};
}

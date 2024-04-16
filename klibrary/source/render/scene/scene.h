#pragma once

#include "render/scene/camera.h"
#include "render/scene/entity.h"

#include "render/light/ambient_light.h"
#include "render/light/directional_light.h"
#include "render/light/point_light.h"


#ifdef KL_USING_PHYSX

namespace kl {
    class Scene
    {
        static physx::PxDefaultAllocator m_allocator;
        static physx::PxDefaultErrorCallback m_error_callback;
        static physx::PxFoundation* m_foundation;

        physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
        physx::PxPhysics* m_physics = nullptr;
        physx::PxCooking* m_cooking = nullptr;
        physx::PxScene* m_scene = nullptr;

        std::map<std::string, Object<Entity>> m_entities = {};

    public:
        std::map<std::string, Object<Mesh>> meshes = {};
        std::map<std::string, Object<Texture>> textures = {};
        std::map<std::string, Object<Material>> materials = {};

        Object<Camera> camera = new Camera();
        Object<Entity> selected_entity = nullptr;

        Object<AmbientLight> ambient_light = new AmbientLight();
        Object<DirectionalLight> directional_light = nullptr;

        // Creation
        Scene();
        virtual ~Scene();

        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;

        void operator=(const Scene&) = delete;
        void operator=(Scene&&) = delete;

        // Iterate
        std::map<std::string, Object<Entity>>::iterator begin();
        std::map<std::string, Object<Entity>>::iterator end();
        std::map<std::string, Object<Entity>>::const_iterator begin() const;
        std::map<std::string, Object<Entity>>::const_iterator end() const;

        // Get
        physx::PxPhysics* physics() const;
        physx::PxCooking* cooking() const;

        Object<Entity> find_entity(const std::string& name) const;
        std::string find_name(const Object<kl::Entity>& entity) const;

        size_t entity_count() const;

        // Set/Get
        void set_gravity(const Float3& gravity);
        Float3 gravity() const;

        void add(const std::string& name, const Object<kl::Entity>& entity);
        void remove(const std::string& name);

        // Update
        Object<kl::Entity> update_selected_entity(uint32_t index);
        void update_physics(float delta_t);

        // Entity
        Object<kl::Entity> make_entity(bool dynamic) const;

        // Dynamic colliders
        Object<Collider> make_box_collider(const Float3& scale) const;
        Object<Collider> make_sphere_collider(float radius) const;
        Object<Collider> make_capsule_collider(float radius, float height) const;

        // Static colliders
        Object<Collider> make_plane_collider() const;
        Object<Collider> make_mesh_collider(const Mesh& mesh, const Float3& scale) const;

        // Default collider
        Object<Collider> make_default_collider(physx::PxGeometryType::Enum type, const Mesh* optional_mesh) const;
    };
}

#else

namespace kl {
    class Scene : public std::vector<Object<Entity>>
    {
    public:
        Object<Camera> camera = nullptr;
        Object<Entity> selected_entity = nullptr;

        Object<AmbientLight> ambient_light = nullptr;
        Object<DirectionalLight> directional_light = nullptr;

        Float3 gravity = { 0.0f, -9.81f, 0.0f };

        void update_physics(float delta_t);
    };
}

#endif

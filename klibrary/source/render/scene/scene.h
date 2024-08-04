#pragma once

#include "render/scene/camera.h"
#include "render/scene/entity.h"

#include "render/light/ambient_light.h"
#include "render/light/directional_light.h"
#include "render/light/point_light.h"


#ifdef KL_USING_PHYSX

namespace kl {
    class Scene : NoCopy
    {
        static physx::PxDefaultAllocator m_allocator;
        static physx::PxDefaultErrorCallback m_error_callback;
        static physx::PxFoundation* m_foundation;

        physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
        physx::PxPhysics* m_physics = nullptr;
        physx::PxCooking* m_cooking = nullptr;
        physx::PxScene* m_scene = nullptr;

        std::map<std::string, Ref<Entity>> m_entities = {};

    public:
        std::map<std::string, Ref<Mesh>> meshes = {};
        std::map<std::string, Ref<Texture>> textures = {};
        std::map<std::string, Ref<Material>> materials = {};

        Ref<Camera> camera = new Camera();
        Ref<Entity> selected_entity = nullptr;

        Ref<AmbientLight> ambient_light = new AmbientLight();
        Ref<DirectionalLight> directional_light = nullptr;

        // Creation
        Scene();
        virtual ~Scene();

        // Iterate
        std::map<std::string, Ref<Entity>>::iterator begin();
        std::map<std::string, Ref<Entity>>::iterator end();
        std::map<std::string, Ref<Entity>>::const_iterator begin() const;
        std::map<std::string, Ref<Entity>>::const_iterator end() const;

        // Get
        physx::PxPhysics* physics() const;
        physx::PxCooking* cooking() const;

        Ref<Entity> find_entity(const std::string& name) const;
        std::string find_name(const Ref<kl::Entity>& entity) const;

        size_t entity_count() const;

        // Set/Get
        void set_gravity(const Float3& gravity);
        Float3 gravity() const;

        void add(const std::string& name, const Ref<kl::Entity>& entity);
        void remove(const std::string& name);

        // Update
        Ref<kl::Entity> update_selected_entity(uint32_t index);
        void update_physics(float delta_t);

        // Entity
        Ref<kl::Entity> make_entity(bool dynamic) const;

        // Dynamic colliders
        Ref<Collider> make_box_collider(const Float3& scale) const;
        Ref<Collider> make_sphere_collider(float radius) const;
        Ref<Collider> make_capsule_collider(float radius, float height) const;

        // Static colliders
        Ref<Collider> make_plane_collider() const;
        Ref<Collider> make_mesh_collider(const Mesh& mesh, const Float3& scale) const;

        // Default collider
        Ref<Collider> make_default_collider(physx::PxGeometryType::Enum type, const Mesh* optional_mesh) const;
    };
}

#else

namespace kl {
    class Scene : public std::vector<Ref<Entity>>
    {
    public:
        Ref<Camera> camera = nullptr;
        Ref<Entity> selected_entity = nullptr;

        Ref<AmbientLight> ambient_light = nullptr;
        Ref<DirectionalLight> directional_light = nullptr;

        Float3 gravity = { 0.0f, -9.81f, 0.0f };

        void update_physics(float delta_t);
    };
}

#endif

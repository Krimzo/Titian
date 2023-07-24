#pragma once

#include "render/scene/camera.h"
#include "render/scene/entity.h"

#include "render/light/ambient_light.h"
#include "render/light/directional_light.h"
#include "render/light/point_light.h"


#ifdef KL_USING_PHYSX

namespace kl {
    class scene
    {
        static PxDefaultAllocator allocator_;
        static PxDefaultErrorCallback error_callback_;
        static PxFoundation* foundation_;

        PxPhysics* physics_ = nullptr;
        PxCooking* cooking_ = nullptr;
        PxDefaultCpuDispatcher* dispatcher_ = nullptr;
        PxScene* scene_ = nullptr;

        std::map<std::string, object<entity>> entities_ = {};

    public:
        std::map<std::string, object<mesh>> meshes = {};
        std::map<std::string, object<texture>> textures = {};
        std::map<std::string, object<material>> materials = {};

        object<camera> camera = nullptr;
        object<entity> selected_entity = nullptr;

        object<ambient_light> ambient_light = nullptr;
        object<directional_light> directional_light = nullptr;

        // Creation
        scene();
        ~scene();

        scene(const scene&) = delete;
        scene(const scene&&) = delete;

        void operator=(const scene&) = delete;
        void operator=(const scene&&) = delete;

        // Iterate
        std::map<std::string, object<entity>>::iterator begin();
        std::map<std::string, object<entity>>::iterator end();

        // Get
        PxPhysics* physics() const;
        PxCooking* cooking() const;

        object<entity> entity(const std::string& name) const;
        std::string name(object<kl::entity> entity) const;

        size_t entity_count() const;

        // Set/Get
        void set_gravity(const float3& gravity);
        float3 gravity() const;

        void add(const std::string& name, object<kl::entity> entity);
        void remove(const std::string& name);

        // Update
        object<kl::entity> update_selected_entity(uint32_t index);
        void update_physics(float delta_t);

        // Entity
        object<kl::entity> make_entity(bool dynamic);

        // Dynamic colliders
        object<collider> make_box_collider(const float3& scale);
        object<collider> make_sphere_collider(float radius);
        object<collider> make_capsule_collider(float radius, float height);

        // Static colliders
        object<collider> make_plane_collider();
        object<collider> make_mesh_collider(const mesh& mesh, const float3& scale);

        // Default collider
        object<collider> make_default_collider(PxGeometryType::Enum type, const mesh* optional_mesh);
    };
}

#else

namespace kl {
    class scene : public std::vector<object<entity>>
    {
    public:
        object<camera> camera = nullptr;
        object<entity> selected_entity = nullptr;

        object<ambient_light> ambient_light = nullptr;
        object<directional_light> directional_light = nullptr;

        float3 gravity = { 0.0f, -9.81f, 0.0f };

        void update_physics(float delta_t);
    };
}

#endif

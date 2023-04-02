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

        std::map<std::string, ref<entity>> entities_ = {};

    public:
        std::map<std::string, ref<mesh>> meshes = {};
        std::map<std::string, ref<texture>> textures = {};
        std::map<std::string, ref<material>> materials = {};

        ref<camera> camera = nullptr;
        ref<entity> selected_entity = nullptr;

        ref<ambient_light> ambient_light = nullptr;
        ref<directional_light> directional_light = nullptr;

        // Creation
        scene();
        ~scene();

        scene(const scene&) = delete;
        scene(const scene&&) = delete;

        void operator=(const scene&) = delete;
        void operator=(const scene&&) = delete;

        // Iterate
        std::map<std::string, ref<entity>>::iterator begin();
        std::map<std::string, ref<entity>>::iterator end();

        // Get
        PxPhysics* get_physics() const;
        PxCooking* get_cooking() const;

        ref<entity> get_entity(const std::string& name) const;
        std::string get_name(ref<entity> entity) const;

        size_t entity_count() const;

        // Set/Get
        void set_gravity(const float3& gravity);
        float3 get_gravity() const;

        void add(const std::string& name, ref<entity> entity);
        void remove(const std::string& name);

        // Update
        ref<entity> update_selected_entity(uint32_t index);
        void update_physics(float delta_t);

        // Entity
        ref<entity> make_entity(bool dynamic);

        // Dynamic colliders
        ref<collider> make_box_collider(const float3& scale);
        ref<collider> make_sphere_collider(float radius);
        ref<collider> make_capsule_collider(float radius, float height);

        // Static colliders
        ref<collider> make_plane_collider();
        ref<collider> make_mesh_collider(const mesh& mesh, const float3& scale);

        // Default collider
        ref<collider> make_default_collider(PxGeometryType::Enum type, const mesh* optional_mesh);
    };
}

#else

namespace kl {
    class scene : public std::unordered_set<ref<entity>>
    {
    public:
        ref<camera> camera = make<kl::camera>();
        ref<entity> selected_entity = nullptr;

        ref<ambient_light> ambient_light = make<kl::ambient_light>();
        ref<directional_light> directional_light = nullptr;

        float3 gravity = { 0.0f, -9.81f, 0.0f };

        scene();
        ~scene();

        void update_physics(float delta_t);
    };
}

#endif

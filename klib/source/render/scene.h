#pragma once

#include "render/camera.h"

#include "render/entity/entity.h"

#include "render/light/ambient_light.h"
#include "render/light/directional_light.h"

#include "memory/ref.h"

#include <map>


namespace kl {
#ifdef KL_USING_PHYSX

    class scene
    {
        std::map<std::string, ref<entity>> entities_;

        static PxDefaultAllocator allocator_;
        static PxDefaultErrorCallback error_callback_;
        static PxFoundation* foundation_;

        PxPhysics* physics_ = nullptr;
        PxCooking* cooking_ = nullptr;
        PxDefaultCpuDispatcher* dispatcher_ = nullptr;
        PxScene* scene_ = nullptr;

    public:
        camera camera = {};
        ref<entity> selected_entity = {};

        ref<ambient_light> ambient_light = {};
        ref<directional_light> directional_light = {};

        dx::buffer ocean_mesh = nullptr;

        scene();
        ~scene();

        scene(const scene&) = delete;
        scene(const scene&&) = delete;

        void operator=(const scene&) = delete;
        void operator=(const scene&&) = delete;

        ref<entity> update_selected_entity(uint32_t index);

        PxPhysics* get_physics() const;
        PxCooking* get_cooking() const;

        // Scene properties
        void set_gravity(const float3& gravity);
        float3 get_gravity() const;

        std::map<std::string, ref<entity>>::iterator begin();
        std::map<std::string, ref<entity>>::iterator end();

        void add(const std::string& name, ref<entity> entity);
        void remove(const std::string& name);

        int entity_count() const;

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
    };

#else

    class scene : public std::set<ref<entity>>
    {
    public:
        camera camera = {};

        ref<ambient_light>         ambient_light = {};
        ref<directional_light> directional_light = {};

        float3 gravity = { 0.0f, -9.81f, 0.0f };

        scene();
        ~scene();

        void update_physics(float delta_t);
    };

#endif
}

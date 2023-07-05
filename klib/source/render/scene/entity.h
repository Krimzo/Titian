#pragma once

#include "render/components/mesh.h"
#include "render/components/material.h"
#include "render/components/collider.h"


#ifdef KL_USING_PHYSX

namespace kl {
    class entity
    {
        PxRigidActor* physics_actor_ = nullptr;
        object<collider> collider_ = nullptr;

        void regenerate_actor(PxPhysics* physics, const PxTransform& transform, bool dynamic);
        void wake_up() const;

    public:
        const uint64_t unique_index;
        float3 render_scale = float3(1.0f);

        object<mesh> mesh = nullptr;
        object<material> material = nullptr;

        // Creation
        entity(PxPhysics* physics, bool dynamic);
        ~entity();

        entity(const entity&) = delete;
        entity(const entity&&) = delete;

        void operator=(const entity&) = delete;
        void operator=(const entity&&) = delete;

        // Get
        PxRigidActor* get_actor() const;
        float4x4 matrix() const;
        float4x4 collider_matrix() const;

        // Geometry
        void set_rotation(const float3& rotation);
        float3 get_rotation() const;

        void set_position(const float3& position);
        float3 get_position() const;

        // Physics
        void set_dynamic(PxPhysics* physics, bool enabled);
        bool is_dynamic() const;

        void set_gravity(bool enabled);
        bool has_gravity() const;

        void set_mass(float mass);
        float get_mass() const;

        void set_velocity(const float3& velocity);
        float3 get_velocity() const;

        void set_angular(const float3& angular);
        float3 get_angular() const;

        // Collision
        void set_collider(object<collider> collider);
        object<collider> get_collider() const;
    };
}

#else

namespace kl {
    class entity
    {
    public:
        float3 scale = float3(1.0f);
        float3 rotation = {};
        float3 position = {};

        float3 acceleration = {};
        float3 velocity = {};
        float3 angular = {};

        object<mesh> mesh = nullptr;
        object<material> material = nullptr;

        void update_physics(float delta_t);

        float4x4 matrix() const;
    };
}

#endif

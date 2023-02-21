#pragma once

#include "render/entity/mesh.h"
#include "render/entity/material.h"
#include "render/entity/collider.h"
#include "memory/memory.h"


namespace kl {
#ifdef KL_USING_PHYSX

    class entity
    {
        const bool is_dynamic_;
        PxRigidActor* physics_actor_ = nullptr;
        ref<collider> collider_ = nullptr;

    public:
        const int unique_index;
        float3 render_scale = float3(1.0f);

        ref<mesh> mesh = nullptr;
        ref<material> material = nullptr;

        // Creation
        entity(PxPhysics* physics, bool dynamic);
        virtual ~entity();

        entity(const entity&) = delete;
        entity(const entity&&) = delete;

        void operator=(const entity&) = delete;
        void operator=(const entity&&) = delete;

        // Get
        PxRigidActor* get_actor() const;

        // Geometry
        void set_rotation(const float3& rotation);
        float3 get_rotation() const;

        void set_position(const float3& position);
        float3 get_position() const;

        // Physics
        void set_mass(float mass);
        float get_mass() const;

        void set_velocity(const float3& velocity);
        float3 get_velocity() const;

        void set_angular(const float3& angular);
        float3 get_angular() const;

        void set_gravity(bool enabled);
        bool get_gravity() const;

        // Collision
        void set_collider(ref<collider> collider);
        ref<collider> get_collider() const;

        // Graphics
        float4x4 matrix() const;
    };
#else

    class entity
    {
    public:
        float3 scale = float3(1.0f);
        float3 rotation = {};
        float3 position = {};

        float3 acceleration = {};
        float3 velocity = {};
        float3 angular = {};

        mesh mesh = nullptr;
        ref<material> material = make<kl::material>();

        entity();
        ~entity();

        void update_physics(float delta_t);

        float4x4 matrix() const;
    };

#endif
}

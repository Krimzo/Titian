#pragma once

#include "render/entity/mesh.h"
#include "render/entity/material.h"
#include "render/entity/collider.h"

#include "memory/ref.h"


namespace kl {
#ifdef KL_USING_PHYSX

    class entity
    {
        const bool is_dynamic_;
        PxRigidActor* physics_actor_ = nullptr;
        ref<collider> collider_ = nullptr;

    public:
        const int unique_index;
        float3 render_scale = float3::splash(1.0f);

        ref<mesh> mesh = {};
        ref<material> material = {};

        entity(PxPhysics* physics, bool dynamic);
        entity();

        ~entity();

        entity(const entity&) = delete;
        entity(const entity&&) = delete;

        void operator=(const entity&) = delete;
        void operator=(const entity&&) = delete;

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
        mat4 matrix() const;
    };
#else

    class entity
    {
    public:
        float3 scale = float3::splash(1.0f);
        float3 rotation = {};
        float3 position = {};

        float3 acceleration = {};
        float3 velocity = {};
        float3 angular = {};

        mesh mesh = {};
        material material = {};

        entity();
        ~entity();

        mat4 matrix() const;
    };

#endif
}

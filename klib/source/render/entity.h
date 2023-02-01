#pragma once

#include "render/mesh.h"
#include "render/material.h"
#include "math/matrix/matrix4x4.h"

#include "memory/ref.h"
#include "btBulletDynamicsCommon.h"


namespace kl {
    class entity
    {
        kl::ref<btCollisionShape> collider_shape_ = {};
        kl::ref<btMotionState> motion_state_ = {};
        kl::ref<btRigidBody> physics_body_ = {};

    public:
        mesh mesh = {};
        material material = {};

        entity();

        btRigidBody* get_body() const;

        // Geometry
        void set_scale(const float3& scale);
        float3 get_scale() const;

        void set_rotation(const float3& rotation);
        float3 get_rotation() const;

        void set_position(const float3& position);
        float3 get_position() const;

        // Physics
        void set_mass(float mass);
        float get_mass() const;

        void set_friction(float friction);
        float get_friction() const;

        void set_velocity(const float3& velocity);
        float3 get_velocity() const;

        void set_angular(const float3& angular);
        float3 get_angular() const;

        void clear_gravity();
        void set_gravity(const float3& gravity);
        float3 get_gravity() const;

        // Collision
        void set_collider(kl::ref<btCollisionShape> collider);
        kl::ref<btCollisionShape> get_collider() const;

        // Graphics
        mat4 matrix() const;
    };
}

#pragma once

#include "render/components/mesh.h"
#include "render/components/material.h"
#include "render/components/collider.h"


#ifdef KL_USING_PHYSX

namespace kl {
    class Entity
    {
        physx::PxRigidActor* m_physics_actor = nullptr;
        Object<Collider> m_collider = nullptr;

        void generate_actor(physx::PxPhysics* physics, const physx::PxTransform& transform, bool dynamic);
        void wake_up() const;

    public:
        const uint64_t unique_index;
        Float3 render_scale { 1.0f };

        Object<Mesh> mesh = nullptr;
        Object<Material> material = nullptr;

        // Creation
        Entity(physx::PxPhysics* physics, bool dynamic);
        virtual ~Entity();

        Entity(const Entity&) = delete;
        Entity(const Entity&&) = delete;

        void operator=(const Entity&) = delete;
        void operator=(const Entity&&) = delete;

        // Get
        physx::PxRigidActor* actor() const;
        Float4x4 matrix() const;
        Float4x4 collider_matrix() const;

        // Geometry
        void set_rotation(const Float3& rotation);
        Float3 rotation() const;

        void set_position(const Float3& position);
        Float3 position() const;

        // Physics
        void set_dynamic(physx::PxPhysics* physics, bool enabled);
        bool is_dynamic() const;

        void set_gravity(bool enabled);
        bool has_gravity() const;

        void set_mass(float mass);
        float mass() const;

        void set_velocity(const Float3& velocity);
        Float3 velocity() const;

        void set_angular(const Float3& angular);
        Float3 angular() const;

        // Collision
        void set_collider(const Object<Collider>& collider);
        Object<Collider> collider() const;
    };
}

#else

namespace kl {
    class Entity
    {
    public:
        Float3 scale { 1.0f };
        Float3 rotation = {};
        Float3 position = {};

        Float3 acceleration = {};
        Float3 velocity = {};
        Float3 angular = {};

        Object<Mesh> mesh = nullptr;
        Object<Material> material = nullptr;

        void update_physics(float delta_t);

        Float4x4 matrix() const;
    };
}

#endif

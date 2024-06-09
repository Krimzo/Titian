#pragma once

#include "components/collider.h"


namespace titian {
    enum class EntityType : int32_t
    {
        BASIC,
        CAMERA,
        AMBIENT_LIGHT,
        POINT_LIGHT,
        DIRECTIONAL_LIGHT,
    };
}

namespace titian {
    class Entity : public Serializable
    {
    public:
        const EntityType type;

        kl::Float3 scale{ 1.0f };
        bool casts_shadows = true;

        std::string mesh_name = "/";
        std::string material_name = "/";

        // Creation
        Entity(EntityType type, physx::PxPhysics* physics, bool dynamic);
        ~Entity() override;

        Entity(const Entity&) = delete;
        Entity(Entity&&) = delete;

        void operator=(const Entity&) = delete;
        void operator=(Entity&&) = delete;

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        // Get
        physx::PxRigidActor* actor() const;
        kl::Float4x4 model_matrix() const;
        kl::Float4x4 collider_matrix() const;

        // Geometry
        void set_rotation(const kl::Float3& rotation);
        kl::Float3 rotation() const;

        void set_position(const kl::Float3& position);
        kl::Float3 position() const;

        // Physics
        void set_dynamic(bool enabled);
        bool is_dynamic() const;

        void set_gravity(bool enabled);
        bool has_gravity() const;

        void set_mass(float mass);
        float mass() const;

        void set_velocity(const kl::Float3& velocity);
        kl::Float3 velocity() const;

        void set_angular(const kl::Float3& angular);
        kl::Float3 angular() const;

        // Collision
        void set_collider(const kl::Object<Collider>& collider);
        kl::Object<Collider> collider() const;

    private:
        physx::PxPhysics* m_physics = nullptr;
        physx::PxRigidActor* m_actor = nullptr;
        kl::Object<Collider> m_collider = nullptr;

        void cleanup();
        void generate_actor(const physx::PxTransform& transform, bool dynamic);
        void wake_up() const;
    };
}

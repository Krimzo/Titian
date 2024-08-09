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
    class Entity : kl::NoCopy, public Serializable
    {
    public:
        const EntityType entity_type;

        Float3 scale{ 1.0f };
        bool casts_shadows = true;

        String animation_name = "/";
        String material_name = "/";
        String collider_mesh_name = "/";

        // Creation
        Entity(EntityType type, px::PxPhysics* physics, bool dynamic);
        ~Entity() override;

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        // Get
        px::PxRigidActor* actor() const;
        Float4x4 model_matrix() const;
        Float4x4 collider_matrix() const;

        // Geometry
        void set_rotation(const Float3& rotation);
        Float3 rotation() const;

        void set_position(const Float3& position);
        Float3 position() const;

        // Physics
        void set_dynamic(bool enabled);
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
        void set_collider(const Ref<Collider>& collider);
        Ref<Collider> collider() const;

    private:
        px::PxPhysics* m_physics = nullptr;
        px::PxRigidActor* m_actor = nullptr;
        Ref<Collider> m_collider = nullptr;

        void cleanup();
        void generate_actor(const px::PxTransform& transform, bool dynamic);
        void wake_up() const;
    };
}

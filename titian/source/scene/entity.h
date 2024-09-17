﻿#pragma once

#include "components/collider.h"


namespace titian {
    struct Entity : kl::NoCopy, Serializable
    {
        bool casts_shadows = true;

        String animation_name = "/";
        String material_name = "/";

        Entity();
        ~Entity() override;

        void serialize(Serializer* serializer) const override;
        void deserialize(const Serializer* serializer) override;

        px::PxRigidActor* actor() const;

        void set_dynamic(bool enabled);
        bool dynamic() const;

        void set_gravity(bool enabled);
        bool gravity() const;

        void set_mass(float mass);
        float mass() const;

        void set_angular_damping(float damping);
		float angular_damping() const;

        void set_scale(const Float3& scale);
		Float3 scale() const;

        void set_rotation(const Float3& rotation);
        Float3 rotation() const;

        void set_position(const Float3& position);
        Float3 position() const;

        void set_velocity(const Float3& velocity);
        Float3 velocity() const;

        void set_angular(const Float3& angular);
        Float3 angular() const;

        void set_collider(const Ref<Collider>& collider);
        Ref<Collider> collider() const;

        Float4x4 model_matrix() const;
        Float4x4 collider_matrix() const;

        Ref<Entity> clone() const;

    private:
        Float3 m_scale{ 1.0f };
        px::PxRigidActor* m_actor = nullptr;
        Ref<Collider> m_collider;

        void generate_actor(const px::PxTransform& transform, bool dynamic);
    };
}

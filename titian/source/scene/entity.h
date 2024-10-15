#pragma once

#include "components/mesh.h"


namespace titian {
    struct Entity : kl::NoCopy, Serializable
    {
        bool shadows = true;
        String animation_name = "/";
        String material_name = "/";

        Entity();
        ~Entity() override;

        void serialize(Serializer& serializer) const override;
        void deserialize(const Serializer& serializer) override;

        px::PxRigidDynamic& actor() const;
        px::PxGeometryType::Enum geometry_type() const;

        void wake_up();

        void set_dynamic(bool enabled);
        bool dynamic() const;

        void set_gravity(bool enabled);
        bool gravity() const;

        void set_mass(float mass);
        float mass() const;

        void set_angular_damping(float damping);
		float angular_damping() const;

        void set_static_friction(float friction);
        float static_friction() const;

        void set_dynamic_friction(float friction);
        float dynamic_friction() const;

        void set_restitution(float restitution);
        float restitution() const;

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

        void set_collider_rotation(const Float3& rotation);
        Float3 collider_rotation() const;

        void set_collider_offset(const Float3& position);
        Float3 collider_offset() const;

        void set_collider_geometry(const px::PxGeometry& geometry);
        px::PxGeometryHolder collider_geometry() const;

        void set_box_collider(const Float3& scale);
        void set_sphere_collider(float radius);
        void set_capsule_collider(float radius, float height);
        void remove_collider();

        Float4x4 model_matrix() const;
        Float4x4 collider_matrix() const;

        Ref<Entity> clone() const;

    private:
        Float3 m_scale{ 1.0f };
        px::PxRigidDynamic& m_actor;
        px::PxMaterial& m_material;

        px::PxShape* collider_shape() const;
        Float4x4 collider_scaling_matrix() const;
    };
}

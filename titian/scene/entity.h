#pragma once

#include "components/mesh.h"

namespace titian
{
struct Entity : kl::NoCopy, Serializable
{
    bool shadows = true;
    String animation_name = "/";
    String material_name = "/";

    Entity();
    ~Entity() override;

    void serialize(Serializer& serializer) const override;
    void deserialize(Serializer const& serializer) override;

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

    void set_scale(float3 const& scale);
    float3 scale() const;

    void set_rotation(float3 const& rotation);
    float3 rotation() const;

    void set_position(float3 const& position);
    float3 position() const;

    void set_velocity(float3 const& velocity);
    float3 velocity() const;

    void set_angular(float3 const& angular);
    float3 angular() const;

    void set_collider_rotation(float3 const& rotation);
    float3 collider_rotation() const;

    void set_collider_offset(float3 const& position);
    float3 collider_offset() const;

    void set_collider_geometry(px::PxGeometry const& geometry);
    Opt<px::PxGeometryHolder> collider_geometry() const;

    void set_box_collider(float3 const& scale);
    void set_sphere_collider(float radius);
    void set_capsule_collider(float radius, float height);
    void remove_collider();

    float4x4 model_matrix() const;
    float4x4 collider_matrix() const;

    Ref<Entity> clone() const;

  private:
    float3 m_scale{1.0f};
    px::PxRigidDynamic& m_actor;
    px::PxMaterial& m_material;

    px::PxShape* collider_shape() const;
    float4x4 collider_scaling_matrix() const;
};
} // namespace titian

#include "titian.h"

static const px::PxTransform DEFAULT_TRANSFORM = {
    px::PxVec3{px::PxZero},
    px::PxQuat{px::PxIdentity},
};

titian::Entity::Entity()
    : m_actor(*AppLayer::get().physics.createRigidDynamic(DEFAULT_TRANSFORM)),
      m_material(*AppLayer::get().physics.createMaterial(0.25f, 0.25f, 0.25f))
{
    m_actor.userData = this;
    set_dynamic(false);
    set_gravity(false);
    set_mass(1.0f);
    set_angular_damping(0.0f);
}

titian::Entity::~Entity()
{
    if (auto scene = m_actor.getScene())
        scene->removeActor(m_actor);
    m_material.release();
    m_actor.release();
}

void titian::Entity::serialize(Serializer& serializer) const
{
    serializer.write_string("entity_type", typeid(*this).name());

    serializer.write_bool("shadows", shadows);
    serializer.write_string("animation_name", animation_name);
    serializer.write_string("material_name", material_name);

    serializer.write_bool("dynamic", dynamic());
    serializer.write_bool("gravity", gravity());
    serializer.write_float("mass", mass());
    serializer.write_float("angular_damping", angular_damping());
    serializer.write_float("static_friction", static_friction());
    serializer.write_float("dynamic_friction", dynamic_friction());
    serializer.write_float("restitution", restitution());

    float3 scale = this->scale();
    serializer.write_float_array("scale", &scale.x, 3);

    float3 rotation = this->rotation();
    serializer.write_float_array("rotation", &rotation.x, 3);

    float3 position = this->position();
    serializer.write_float_array("position", &position.x, 3);

    float3 velocity = this->velocity();
    serializer.write_float_array("velocity", &velocity.x, 3);

    float3 angular = this->angular();
    serializer.write_float_array("angular", &angular.x, 3);

    px::PxGeometryType::Enum geometry_type = this->geometry_type();
    serializer.write_int("geometry_type", geometry_type);

    auto shape = collider_shape();

    serializer.push_object("geometry");
    switch (geometry_type)
    {
    case px::PxGeometryType::Enum::eBOX: {
        px::PxBoxGeometry geometry{};
        if (shape)
            geometry = static_cast<px::PxBoxGeometry const&>(shape->getGeometry());
        serializer.write_float_array("half_extents", &geometry.halfExtents.x, 3);
        break;
    }
    case px::PxGeometryType::Enum::eSPHERE: {
        px::PxSphereGeometry geometry{};
        if (shape)
            geometry = static_cast<px::PxSphereGeometry const&>(shape->getGeometry());
        serializer.write_float("radius", geometry.radius);
        break;
    }
    case px::PxGeometryType::Enum::eCAPSULE: {
        px::PxCapsuleGeometry geometry{};
        if (shape)
            geometry = static_cast<px::PxCapsuleGeometry const&>(shape->getGeometry());
        serializer.write_float("half_height", geometry.halfHeight);
        serializer.write_float("radius", geometry.radius);
        break;
    }
    }
    serializer.pop_object();

    float3 collider_rotation = this->collider_rotation();
    serializer.write_float_array("collider_rotation", &collider_rotation.x, 3);

    float3 collider_offset = this->collider_offset();
    serializer.write_float_array("collider_offset", &collider_offset.x, 3);
}

void titian::Entity::deserialize(Serializer const& serializer)
{
    serializer.read_bool("shadows", shadows);
    serializer.read_string("animation_name", animation_name);
    serializer.read_string("material_name", material_name);

    bool dynamic = false;
    serializer.read_bool("dynamic", dynamic);
    set_dynamic(dynamic);

    bool gravity = false;
    serializer.read_bool("gravity", gravity);
    set_gravity(gravity);

    float mass = 0.0f;
    serializer.read_float("mass", mass);
    set_mass(mass);

    float angular_damping = 0.0f;
    serializer.read_float("angular_damping", angular_damping);
    set_angular_damping(angular_damping);

    float static_friction = 0.0f;
    serializer.read_float("static_friction", static_friction);
    set_static_friction(static_friction);

    float dynamic_friction = 0.0f;
    serializer.read_float("dynamic_friction", dynamic_friction);
    set_dynamic_friction(dynamic_friction);

    float restitution = 0.0f;
    serializer.read_float("restitution", restitution);
    set_restitution(restitution);

    float3 scale;
    serializer.read_float_array("scale", &scale.x, 3);
    set_scale(scale);

    float3 rotation;
    serializer.read_float_array("rotation", &rotation.x, 3);
    set_rotation(rotation);

    float3 position;
    serializer.read_float_array("position", &position.x, 3);
    set_position(position);

    float3 velocity;
    serializer.read_float_array("velocity", &velocity.x, 3);
    set_velocity(velocity);

    float3 angular;
    serializer.read_float_array("angular", &angular.x, 3);
    set_angular(angular);

    px::PxGeometryType::Enum geometry_type{};
    serializer.read_int("geometry_type", (int32_t&)geometry_type);

    serializer.load_object("geometry");
    switch (geometry_type)
    {
    case px::PxGeometryType::Enum::eBOX: {
        px::PxBoxGeometry geometry{};
        serializer.read_float_array("half_extents", &geometry.halfExtents.x, 3);
        set_collider_geometry(geometry);
        break;
    }
    case px::PxGeometryType::Enum::eSPHERE: {
        px::PxSphereGeometry geometry{};
        serializer.read_float("radius", geometry.radius);
        set_collider_geometry(geometry);
        break;
    }
    case px::PxGeometryType::Enum::eCAPSULE: {
        px::PxCapsuleGeometry geometry{};
        serializer.read_float("half_height", geometry.halfHeight);
        serializer.read_float("radius", geometry.radius);
        set_collider_geometry(geometry);
        break;
    }
    }
    serializer.unload_object();

    float3 collider_rotation;
    serializer.read_float_array("collider_rotation", &collider_rotation.x, 3);
    set_collider_rotation(collider_rotation);

    float3 collider_offset;
    serializer.read_float_array("collider_offset", &collider_offset.x, 3);
    set_collider_offset(collider_offset);
}

px::PxRigidDynamic& titian::Entity::actor() const
{
    return m_actor;
}

px::PxGeometryType::Enum titian::Entity::geometry_type() const
{
    if (auto shape = collider_shape())
        return shape->getGeometry().getType();
    return px::PxGeometryType::eINVALID;
}

void titian::Entity::wake_up()
{
    if (m_actor.getScene())
        m_actor.wakeUp();
}

void titian::Entity::set_dynamic(bool enabled)
{
    m_actor.setRigidBodyFlag(px::PxRigidBodyFlag::eKINEMATIC, !enabled);
    wake_up();
}

bool titian::Entity::dynamic() const
{
    px::PxRigidBodyFlags flags = m_actor.getRigidBodyFlags();
    return !flags.isSet(px::PxRigidBodyFlag::eKINEMATIC);
}

void titian::Entity::set_gravity(bool enabled)
{
    m_actor.setActorFlag(px::PxActorFlag::eDISABLE_GRAVITY, !enabled);
    wake_up();
}

bool titian::Entity::gravity() const
{
    px::PxActorFlags flags = m_actor.getActorFlags();
    return !flags.isSet(px::PxActorFlag::eDISABLE_GRAVITY);
}

void titian::Entity::set_mass(float mass)
{
    m_actor.setMass(mass);
    wake_up();
}

float titian::Entity::mass() const
{
    return m_actor.getMass();
}

void titian::Entity::set_angular_damping(float damping)
{
    m_actor.setAngularDamping(damping);
}

float titian::Entity::angular_damping() const
{
    return m_actor.getAngularDamping();
}

void titian::Entity::set_static_friction(float friction)
{
    m_material.setStaticFriction(friction);
}

float titian::Entity::static_friction() const
{
    return m_material.getStaticFriction();
}

void titian::Entity::set_dynamic_friction(float friction)
{
    m_material.setDynamicFriction(friction);
}

float titian::Entity::dynamic_friction() const
{
    return m_material.getDynamicFriction();
}

void titian::Entity::set_restitution(float restitution)
{
    m_material.setRestitution(restitution);
}

float titian::Entity::restitution() const
{
    return m_material.getRestitution();
}

void titian::Entity::set_scale(float3 const& scale)
{
    m_scale = scale;
}

float3 titian::Entity::scale() const
{
    return m_scale;
}

void titian::Entity::set_rotation(float3 const& rotation)
{
    px::PxTransform transform = m_actor.getGlobalPose();
    transform.q = px_cast(float4{kl::to_quat(rotation)});
    m_actor.setGlobalPose(transform);
}

float3 titian::Entity::rotation() const
{
    px::PxTransform transform = m_actor.getGlobalPose();
    return kl::to_euler(quat{px_cast(transform.q)});
}

void titian::Entity::set_position(float3 const& position)
{
    px::PxTransform transform = m_actor.getGlobalPose();
    transform.p = px_cast(position);
    m_actor.setGlobalPose(transform);
}

float3 titian::Entity::position() const
{
    px::PxTransform transform = m_actor.getGlobalPose();
    return px_cast(transform.p);
}

void titian::Entity::set_velocity(float3 const& velocity)
{
    m_actor.setLinearVelocity(px_cast(velocity));
}

float3 titian::Entity::velocity() const
{
    return px_cast(m_actor.getLinearVelocity());
}

void titian::Entity::set_angular(float3 const& angular)
{
    m_actor.setAngularVelocity(px_cast(angular));
}

float3 titian::Entity::angular() const
{
    return px_cast(m_actor.getAngularVelocity());
}

void titian::Entity::set_collider_rotation(float3 const& rotation)
{
    auto shape = collider_shape();
    if (!shape)
        return;

    px::PxTransform transform = shape->getLocalPose();
    transform.q = px_cast(float4{kl::to_quat(rotation)});
    shape->setLocalPose(transform);
}

float3 titian::Entity::collider_rotation() const
{
    auto shape = collider_shape();
    if (!shape)
        return {};

    px::PxTransform transform = shape->getLocalPose();
    return kl::to_euler(quat{px_cast(transform.q)});
}

void titian::Entity::set_collider_offset(float3 const& position)
{
    auto shape = collider_shape();
    if (!shape)
        return;

    px::PxTransform transform = shape->getLocalPose();
    transform.p = px_cast(position);
    shape->setLocalPose(transform);
}

float3 titian::Entity::collider_offset() const
{
    auto shape = collider_shape();
    if (!shape)
        return {};

    px::PxTransform transform = shape->getLocalPose();
    return px_cast(transform.p);
}

void titian::Entity::set_collider_geometry(px::PxGeometry const& geometry)
{
    px::PxTransform old_transform = DEFAULT_TRANSFORM;
    if (auto shape = collider_shape())
    {
        old_transform = shape->getLocalPose();
        m_actor.detachShape(*shape);
    }
    px::PxRigidActorExt::createExclusiveShape(m_actor, geometry, m_material);
    if (auto shape = collider_shape())
        shape->setLocalPose(old_transform);
}

titian::Opt<px::PxGeometryHolder> titian::Entity::collider_geometry() const
{
    if (auto shape = collider_shape())
        return shape->getGeometry();
    else
        return std::nullopt;
}

void titian::Entity::set_box_collider(float3 const& scale)
{
    set_collider_geometry(px::PxBoxGeometry{px_cast(scale) * 0.5f});
}

void titian::Entity::set_sphere_collider(float radius)
{
    set_collider_geometry(px::PxSphereGeometry{radius});
}

void titian::Entity::set_capsule_collider(float radius, float height)
{
    set_collider_geometry(px::PxCapsuleGeometry{radius, height});
}

void titian::Entity::remove_collider()
{
    if (auto shape = collider_shape())
        m_actor.detachShape(*shape);
}

float4x4 titian::Entity::model_matrix() const
{
    return float4x4::translation(position()) * float4x4::rotation(rotation()) * float4x4::scaling(scale());
}

float4x4 titian::Entity::collider_matrix() const
{
    return float4x4::translation(position()) * float4x4::rotation(rotation()) *
           float4x4::translation(collider_offset()) * float4x4::rotation(collider_rotation()) *
           collider_scaling_matrix();
}

titian::Ref<titian::Entity> titian::Entity::clone() const
{
    Ref entity = new Entity();
    entity->shadows = shadows;
    entity->animation_name = animation_name;
    entity->material_name = material_name;
    entity->set_dynamic(dynamic());
    entity->set_gravity(gravity());
    entity->set_mass(mass());
    entity->set_angular_damping(angular_damping());
    entity->set_static_friction(static_friction());
    entity->set_dynamic_friction(dynamic_friction());
    entity->set_restitution(restitution());
    entity->set_scale(scale());
    entity->set_rotation(rotation());
    entity->set_position(position());
    entity->set_velocity(velocity());
    entity->set_angular(angular());
    entity->set_collider_rotation(collider_rotation());
    entity->set_collider_offset(collider_offset());
    if (auto geometry = collider_geometry())
        entity->set_collider_geometry(geometry->any());
    return entity;
}

px::PxShape* titian::Entity::collider_shape() const
{
    px::PxShape* shape = nullptr;
    m_actor.getShapes(&shape, 1);
    return shape;
}

float4x4 titian::Entity::collider_scaling_matrix() const
{
    auto shape = collider_shape();
    if (!shape)
        return {};

    switch (shape->getGeometry().getType())
    {
    case px::PxGeometryType::Enum::eBOX: {
        auto const& geometry = static_cast<px::PxBoxGeometry const&>(shape->getGeometry());
        return float4x4::scaling(px_cast(geometry.halfExtents) * 2.0f);
    }
    case px::PxGeometryType::Enum::eSPHERE: {
        auto const& geometry = static_cast<px::PxSphereGeometry const&>(shape->getGeometry());
        return float4x4::scaling(float3{geometry.radius});
    }
    case px::PxGeometryType::Enum::eCAPSULE: {
        auto const& geometry = static_cast<px::PxCapsuleGeometry const&>(shape->getGeometry());
        return float4x4::scaling(float3{geometry.halfHeight * 0.5f, float2{geometry.radius}});
    }
    }
    return {};
}

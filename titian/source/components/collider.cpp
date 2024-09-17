#include "titian.h"


titian::Collider::Collider()
    : m_material(Layers::get<AppLayer>()->physics->createMaterial(0.25f, 0.25f, 0.25f))
{
    kl::assert(m_material, "Failed to create collider MATERIAL");
}

titian::Collider::~Collider()
{
    if (m_shape) m_shape->release();
    if (m_material) m_material->release();
}

void titian::Collider::serialize(Serializer* serializer) const
{
    const px::PxGeometryType::Enum geometry_type = this->type();
    serializer->write_int("geometry_type", geometry_type);

    serializer->push_object("geometry");
    switch (geometry_type) {
    case px::PxGeometryType::Enum::eBOX: {
        px::PxBoxGeometry geometry{};
        m_shape->getBoxGeometry(geometry);
        serializer->write_float_array("half_extents", &geometry.halfExtents.x, 3);
        break;
    }
    case px::PxGeometryType::Enum::eSPHERE: {
        px::PxSphereGeometry geometry{};
        m_shape->getSphereGeometry(geometry);
        serializer->write_float("radius", geometry.radius);
        break;
    }
    case px::PxGeometryType::Enum::eCAPSULE: {
        px::PxCapsuleGeometry geometry{};
        m_shape->getCapsuleGeometry(geometry);
        serializer->write_float("half_height", geometry.halfHeight);
        serializer->write_float("radius", geometry.radius);
        break;
    }
    }
    serializer->pop_object();

    const Float3 rotation = this->rotation();
    serializer->write_float_array("rotation", &rotation.x, 3);

    const Float3 offset = this->offset();
    serializer->write_float_array("offset", &offset.x, 3);

    serializer->write_float("static_friction", static_friction());
    serializer->write_float("dynamic_friction", dynamic_friction());
    serializer->write_float("restitution", restitution());
}

void titian::Collider::deserialize(const Serializer* serializer)
{
    px::PxGeometryType::Enum geometry_type{};
    serializer->read_int("geometry_type", (int&) geometry_type);

    serializer->load_object("geometry");
    switch (geometry_type) {
    case px::PxGeometryType::Enum::eBOX: {
        px::PxBoxGeometry geometry{};
        serializer->read_float_array("half_extents", &geometry.halfExtents.x, 3);
        set_geometry(geometry);
        break;
    }
    case px::PxGeometryType::Enum::eSPHERE: {
        px::PxSphereGeometry geometry{};
        serializer->read_float("radius", geometry.radius);
        set_geometry(geometry);
        break;
    }
    case px::PxGeometryType::Enum::eCAPSULE: {
        px::PxCapsuleGeometry geometry{};
        serializer->read_float("half_height", geometry.halfHeight);
        serializer->read_float("radius", geometry.radius);
        set_geometry(geometry);
        break;
    }
    }
    serializer->unload_object();

    Float3 rotation;
    serializer->read_float_array("rotation", &rotation.x, 3);
    set_rotation(rotation);

    Float3 offset;
    serializer->read_float_array("offset", &offset.x, 3);
    set_offset(offset);

    float static_friction = 0.0f;
    serializer->read_float("static_friction", static_friction);
    set_static_friction(static_friction);

    float dynamic_friction = 0.0f;
    serializer->read_float("dynamic_friction", dynamic_friction);
    set_dynamic_friction(dynamic_friction);

    float restitution = 0.0f;
    serializer->read_float("restitution", restitution);
    set_restitution(restitution);
}

px::PxGeometryType::Enum titian::Collider::type() const
{
    return m_shape->getGeometryType();
}

px::PxShape* titian::Collider::shape() const
{
    return m_shape;
}

void titian::Collider::set_geometry(const px::PxGeometry& geometry)
{
    kl::assert(
        geometry.getType() == px::PxGeometryType::Enum::eBOX ||
        geometry.getType() == px::PxGeometryType::Enum::eSPHERE ||
        geometry.getType() == px::PxGeometryType::Enum::eCAPSULE, "Not a valid geometry type");

    px::PxPhysics* physics = Layers::get<AppLayer>()->physics;
    if (m_shape) m_shape->release();
    m_shape = physics->createShape(geometry, *m_material);
}

px::PxGeometryHolder titian::Collider::geometry() const
{
    return m_shape->getGeometry();
}

void titian::Collider::set_rotation(const Float3& rotation)
{
    px::PxTransform transform = m_shape->getLocalPose();
    transform.q = px_cast(Float4{ kl::to_quaternion(rotation) });
    m_shape->setLocalPose(transform);
}

titian::Float3 titian::Collider::rotation() const
{
    const px::PxTransform transform = m_shape->getLocalPose();
    return kl::to_euler(Quaternion{ px_cast(transform.q) });
}

void titian::Collider::set_offset(const Float3& position)
{
    px::PxTransform transform = m_shape->getLocalPose();
    transform.p = px_cast(position);
    m_shape->setLocalPose(transform);
}

titian::Float3 titian::Collider::offset() const
{
    const px::PxTransform transform = m_shape->getLocalPose();
    return px_cast(transform.p);
}

void titian::Collider::set_static_friction(const float friction)
{
    m_material->setStaticFriction(friction);
}

float titian::Collider::static_friction() const
{
    return m_material->getStaticFriction();
}

void titian::Collider::set_dynamic_friction(const float friction)
{
    m_material->setDynamicFriction(friction);
}

float titian::Collider::dynamic_friction() const
{
    return m_material->getDynamicFriction();
}

void titian::Collider::set_restitution(const float restitution)
{
    m_material->setRestitution(restitution);
}

float titian::Collider::restitution() const
{
    return m_material->getRestitution();
}

titian::Float4x4 titian::Collider::scaling_matrix() const
{
    switch (type())
    {
    case px::PxGeometryType::Enum::eBOX:
    {
        px::PxBoxGeometry geometry{};
        m_shape->getBoxGeometry(geometry);
        return Float4x4::scaling(px_cast(geometry.halfExtents) * 2.0f);
    }
    case px::PxGeometryType::Enum::eSPHERE:
    {
        px::PxSphereGeometry geometry{};
        m_shape->getSphereGeometry(geometry);
        return Float4x4::scaling(Float3{ geometry.radius });
    }
    case px::PxGeometryType::Enum::eCAPSULE:
    {
        px::PxCapsuleGeometry geometry{};
        m_shape->getCapsuleGeometry(geometry);
        return Float4x4::scaling(Float3{ geometry.halfHeight * 0.5f, Float2{geometry.radius} });
    }
    }
    return {};
}

titian::Ref<titian::Collider> titian::Collider::clone() const
{
    Ref collider = new Collider();
    collider->set_geometry(geometry().any());
    collider->set_rotation(rotation());
    collider->set_offset(offset());
    collider->set_static_friction(static_friction());
    collider->set_dynamic_friction(dynamic_friction());
    collider->set_restitution(restitution());
    return collider;
}

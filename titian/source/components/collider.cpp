#include "titian.h"


titian::Collider::Collider(px::PxPhysics* physics)
    : m_physics(physics)
{
    m_material = m_physics->createMaterial(0.25f, 0.25f, 0.25f);
    kl::assert(m_material, "Failed to create collider MATERIAL");
}

titian::Collider::Collider(px::PxPhysics* physics, const px::PxGeometry& geometry)
    : Collider(physics)
{
    set_geometry(geometry);
}

titian::Collider::~Collider()
{
    if (m_shape) {
        m_shape->release();
        m_shape = nullptr;
    }
    if (m_material) {
        m_material->release();
        m_material = nullptr;
    }
}

void titian::Collider::serialize(Serializer* serializer, const void* helper_data) const
{
    const auto helper_meshes = static_cast<const Map<String, Ref<Mesh>>*>(helper_data);

    const px::PxGeometryType::Enum type = this->type();
    serializer->write_object<px::PxGeometryType::Enum>(type);

    switch (type) {
    case px::PxGeometryType::Enum::eBOX: {
        px::PxBoxGeometry geometry = {};
        m_shape->getBoxGeometry(geometry);
        serializer->write_object<Float3>(reinterpret_cast<Float3&>(geometry.halfExtents));
        break;
    }
    case px::PxGeometryType::Enum::eSPHERE: {
        px::PxSphereGeometry geometry = {};
        m_shape->getSphereGeometry(geometry);
        serializer->write_object<float>(geometry.radius);
        break;
    }
    case px::PxGeometryType::Enum::eCAPSULE: {
        px::PxCapsuleGeometry geometry = {};
        m_shape->getCapsuleGeometry(geometry);
        serializer->write_object<float>(geometry.halfHeight);
        serializer->write_object<float>(geometry.radius);
        break;
    }
    case px::PxGeometryType::Enum::eTRIANGLEMESH: {
        px::PxTriangleMeshGeometry geometry = {};
        m_shape->getTriangleMeshGeometry(geometry);
        serializer->write_object<Float3>(reinterpret_cast<Float3&>(geometry.scale.scale));
        bool was_found = false;
        for (const auto& [name, mesh] : *helper_meshes) {
            if (mesh->physics_buffer == geometry.triangleMesh) {
                serializer->write_string(name);
                was_found = true;
                break;
            }
        }
        kl::assert(was_found, "Failed to find collider MESH");
        break;
    }
    }

    serializer->write_object<Float3>(rotation());
    serializer->write_object<Float3>(offset());

    serializer->write_object<float>(static_friction());
    serializer->write_object<float>(dynamic_friction());
    serializer->write_object<float>(restitution());
}

void titian::Collider::deserialize(const Serializer* serializer, const void* helper_data)
{
    const auto helper_meshes = static_cast<const Map<String, Ref<Mesh>>*>(helper_data);

    const px::PxGeometryType::Enum type = serializer->read_object<px::PxGeometryType::Enum>();

    switch (type) {
    case px::PxGeometryType::Enum::eBOX: {
        px::PxBoxGeometry geometry = {};
        serializer->read_object<Float3>(reinterpret_cast<Float3&>(geometry.halfExtents));
        this->set_geometry(geometry);
        break;
    }
    case px::PxGeometryType::Enum::eSPHERE: {
        px::PxSphereGeometry geometry = {};
        serializer->read_object<float>(geometry.radius);
        this->set_geometry(geometry);
        break;
    }
    case px::PxGeometryType::Enum::eCAPSULE: {
        px::PxCapsuleGeometry geometry = {};
        serializer->read_object<float>(geometry.halfHeight);
        serializer->read_object<float>(geometry.radius);
        this->set_geometry(geometry);
        break;
    }
    case px::PxGeometryType::Enum::eTRIANGLEMESH: {
        px::PxTriangleMeshGeometry geometry = {};
        serializer->read_object<Float3>(reinterpret_cast<Float3&>(geometry.scale.scale));
        geometry.triangleMesh = (*helper_meshes).at(serializer->read_string())->physics_buffer;
        this->set_geometry(geometry);
        break;
    }
    }

    set_rotation(serializer->read_object<Float3>());
    set_offset(serializer->read_object<Float3>());

    set_static_friction(serializer->read_object<float>());
    set_dynamic_friction(serializer->read_object<float>());
    set_restitution(serializer->read_object<float>());
}

// Get
px::PxShape* titian::Collider::shape() const
{
    return m_shape;
}

px::PxGeometryType::Enum titian::Collider::type() const
{
    return m_shape->getGeometryType();
}

titian::Float4x4 titian::Collider::scaling_matrix() const
{
    switch (type()) {
    case px::PxGeometryType::Enum::eBOX: {
        px::PxBoxGeometry geometry = {};
        m_shape->getBoxGeometry(geometry);
        return Float4x4::scaling(reinterpret_cast<Float3&>(geometry.halfExtents));
    }
    case px::PxGeometryType::Enum::eSPHERE: {
        px::PxSphereGeometry geometry = {};
        m_shape->getSphereGeometry(geometry);
        return Float4x4::scaling(Float3{ geometry.radius });
    }
    case px::PxGeometryType::Enum::eCAPSULE: {
        px::PxCapsuleGeometry geometry = {};
        m_shape->getCapsuleGeometry(geometry);
        return Float4x4::scaling(Float3{ geometry.halfHeight * 0.5f, Float2{geometry.radius} });
    }
    case px::PxGeometryType::Enum::eTRIANGLEMESH: {
        px::PxTriangleMeshGeometry geometry = {};
        m_shape->getTriangleMeshGeometry(geometry);
        return Float4x4::scaling(reinterpret_cast<Float3&>(geometry.scale.scale));
    }
    }
    return {};
}

// Geometry
void titian::Collider::set_geometry(const px::PxGeometry& geometry)
{
    if (m_shape) {
        m_shape->release();
        m_shape = nullptr;
    }
    m_shape = m_physics->createShape(geometry, *m_material);
    kl::verify(m_shape, "Failed to create collider SHAPE");
}

void titian::Collider::set_rotation(const Float3& rotation)
{
    const Float4 quat = kl::to_quaternion(rotation);
    px::PxTransform transform = m_shape->getLocalPose();
    transform.q = (const px::PxQuat&)quat;
    m_shape->setLocalPose(transform);
}

titian::Float3 titian::Collider::rotation() const
{
    const px::PxTransform transform = m_shape->getLocalPose();
    return kl::to_euler<float>((const Float4&) transform.q);
}

void titian::Collider::set_offset(const Float3& position)
{
    px::PxTransform transform = m_shape->getLocalPose();
    transform.p = (const px::PxVec3&)position;
    m_shape->setLocalPose(transform);
}

titian::Float3 titian::Collider::offset() const
{
    const px::PxTransform transform = m_shape->getLocalPose();
    return (const Float3&) transform.p;
}

// Material
void titian::Collider::set_static_friction(float friction)
{
    m_material->setStaticFriction(friction);
}

float titian::Collider::static_friction() const
{
    return m_material->getStaticFriction();
}

void titian::Collider::set_dynamic_friction(float friction)
{
    m_material->setDynamicFriction(friction);
}

float titian::Collider::dynamic_friction() const
{
    return m_material->getDynamicFriction();
}

void titian::Collider::set_restitution(float restitution)
{
    m_material->setRestitution(restitution);
}

float titian::Collider::restitution() const
{
    return m_material->getRestitution();
}

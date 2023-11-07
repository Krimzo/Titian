#include "klibrary.h"


#ifdef KL_USING_PHYSX

kl::Collider::Collider(physx::PxPhysics* physics, const physx::PxGeometry& geometry)
{
	m_material = physics->createMaterial(0.25f, 0.25f, 0.25f);
	assert(m_material, "Failed to create collider material");

	m_shape = physics->createShape(geometry, *m_material);
	assert(m_shape, "Failed to create collider shape");
}

kl::Collider::~Collider()
{
	m_shape->release();
	m_material->release();
}

// Get
physx::PxShape* kl::Collider::shape() const
{
	return m_shape;
}

physx::PxGeometryType::Enum kl::Collider::type() const
{
	return m_shape->getGeometryType();
}

kl::Float4x4 kl::Collider::scaling_matrix() const
{
	switch (type()) {
	case physx::PxGeometryType::Enum::eBOX:
	{
		physx::PxBoxGeometry geometry = {};
		m_shape->getBoxGeometry(geometry);
		return Float4x4::scaling((Float3&) geometry.halfExtents);
	}

	case physx::PxGeometryType::Enum::eSPHERE:
	{
		physx::PxSphereGeometry geometry = {};
		m_shape->getSphereGeometry(geometry);
		return Float4x4::scaling(Float3(geometry.radius));
	}

	case physx::PxGeometryType::Enum::eCAPSULE:
	{
		physx::PxCapsuleGeometry geometry = {};
		m_shape->getCapsuleGeometry(geometry);
		return Float4x4::scaling(Float3(geometry.halfHeight * 0.5f, Float2(geometry.radius)));
	}

	case physx::PxGeometryType::Enum::ePLANE:
	{
		return {};
	}

	case physx::PxGeometryType::Enum::eTRIANGLEMESH:
	{
		physx::PxTriangleMeshGeometry geometry = {};
		m_shape->getTriangleMeshGeometry(geometry);
		return Float4x4::scaling((Float3&) geometry.scale.scale);
	}
	}
	return {};
}

// Geometry
void kl::Collider::set_rotation(const Float3& rotation)
{
	const Float4 quat = to_quaternion(rotation);
	physx::PxTransform transform = m_shape->getLocalPose();
	transform.q = (const physx::PxQuat&) quat;
	m_shape->setLocalPose(transform);
}

kl::Float3 kl::Collider::rotation() const
{
	const physx::PxTransform transform = m_shape->getLocalPose();
	return to_euler((const Float4&) transform.q);
}

void kl::Collider::set_offset(const Float3& position)
{
	physx::PxTransform transform = m_shape->getLocalPose();
	transform.p = (const physx::PxVec3&) position;
	m_shape->setLocalPose(transform);
}

kl::Float3 kl::Collider::offset() const
{
	const physx::PxTransform transform = m_shape->getLocalPose();
	return (const Float3&) transform.p;
}

// Material
void kl::Collider::set_static_friction(const float friction)
{
	m_material->setStaticFriction(friction);
}

float kl::Collider::static_friction() const
{
	return m_material->getStaticFriction();
}

void kl::Collider::set_dynamic_friction(const float friction)
{
	m_material->setDynamicFriction(friction);
}

float kl::Collider::dynamic_friction() const
{
	return m_material->getDynamicFriction();
}

void kl::Collider::set_restitution(const float restitution)
{
	m_material->setRestitution(restitution);
}

float kl::Collider::restitution() const
{
	return m_material->getRestitution();
}

#endif

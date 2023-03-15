#include "render/entity/collider.h"

#include "utility/utility.h"


#ifdef KL_USING_PHYSX

kl::collider::collider(PxPhysics* physics, const PxGeometry& geometry)
	: physics_(physics)
{
	material_ = physics->createMaterial(0.25f, 0.25f, 0.25f);
	error_check(!material_, "Failed to create collider material");

	shape_ = physics->createShape(geometry, *material_);
	error_check(!shape_, "Failed to create collider shape");
}

kl::collider::~collider()
{
	shape_->release();
	material_->release();
}

// Get
PxShape* kl::collider::get_shape()
{
	return shape_;
}

PxGeometryType::Enum kl::collider::get_type() const
{
	return shape_->getGeometryType();
}

// Geometry
void kl::collider::set_rotation(const float3& rotation)
{
	PxTransform transform = shape_->getLocalPose();
	transform.q = math::to_quaternion(rotation);
	shape_->setLocalPose(transform);
}

kl::float3 kl::collider::get_rotation() const
{
	const PxTransform transform = shape_->getLocalPose();
	return math::to_euler(transform.q);
}

void kl::collider::set_offset(const float3& position)
{
	PxTransform transform = shape_->getLocalPose();
	transform.p = position;
	shape_->setLocalPose(transform);
}

kl::float3 kl::collider::get_offset() const
{
	const PxTransform transform = shape_->getLocalPose();
	return transform.p;
}

// Material
float kl::collider::get_static_friction() const
{
	return material_->getStaticFriction();
}

void kl::collider::set_static_friction(const float friction)
{
	material_->setStaticFriction(friction);
}

float kl::collider::get_dynamic_friction() const
{
	return material_->getDynamicFriction();
}

void kl::collider::set_dynamic_friction(const float friction)
{
	material_->setDynamicFriction(friction);
}

float kl::collider::get_restitution() const
{
	return material_->getRestitution();
}

void kl::collider::set_restitution(const float restitution)
{
	material_->setRestitution(restitution);
}

#endif

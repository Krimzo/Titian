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
PxGeometryType::Enum kl::collider::get_type() const
{
	return shape_->getGeometryType();
}

PxShape* kl::collider::get_shape()
{
	return shape_;
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

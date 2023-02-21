#include "render/entity/collider.h"


#ifdef KL_USING_PHYSX

kl::collider::collider(PxPhysics* physics, const PxGeometry& geometry)
{
	material_ = physics->createMaterial(0.25f, 0.25f, 0.25f);
	shape_ = physics->createShape(geometry, *material_);
}

kl::collider::~collider()
{
	shape_->release();
	material_->release();
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

// Shape
PxShape* kl::collider::get_shape()
{
	return shape_;
}

#endif

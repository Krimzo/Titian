#include "render/entity.h"

#include "math/math.h"


kl::entity::entity()
{
    collider_shape_ = kl::make_empty_collider();
    motion_state_ = kl::make<btDefaultMotionState>();
    physics_body_ = kl::make<btRigidBody, btRigidBody::btRigidBodyConstructionInfo>({ 0.0f, &*motion_state_, &*collider_shape_ });

    physics_body_->forceActivationState(DISABLE_DEACTIVATION);
    set_scale(float3::splash(1.0f));
}

btRigidBody* kl::entity::get_body() const
{
    return &*physics_body_;
}

// Geometry
void kl::entity::set_scale(const float3& scale)
{
    collider_shape_->setLocalScaling({ scale.x, scale.y, scale.z });
}

kl::float3 kl::entity::get_scale() const
{
    const btVector3 scaling = collider_shape_->getLocalScaling();
    return { scaling.x(), scaling.y(), scaling.z() };
}

void kl::entity::set_rotation(const float3& rotation)
{
    const kl::float4 quat = kl::math::to_quaternion(rotation);

    btTransform transform = physics_body_->getWorldTransform();
    transform.setRotation({ quat.x, quat.y, quat.z, quat.w });
    physics_body_->setWorldTransform(transform);
}

kl::float3 kl::entity::get_rotation() const
{
    const btQuaternion quat = physics_body_->getWorldTransform().getRotation();
    return kl::math::to_euler(kl::float4 { quat.x(), quat.y(), quat.z(), quat.w() });
}

void kl::entity::set_position(const float3& position)
{
    btTransform transform = physics_body_->getWorldTransform();
    transform.setOrigin({ position.x, position.y, position.z });
    physics_body_->setWorldTransform(transform);
}

kl::float3 kl::entity::get_position() const
{
    const btTransform transform = physics_body_->getWorldTransform();
    const btVector3 position = transform.getOrigin();
    return { position.x(), position.y(), position.z() };
}

// Physics
void kl::entity::set_mass(float mass)
{
    btVector3 inertia = {};
    collider_shape_->calculateLocalInertia(mass, inertia);
    physics_body_->setMassProps(mass, inertia);
}

float kl::entity::get_mass() const
{
    return physics_body_->getMass();
}

void kl::entity::set_friction(float friction)
{
    physics_body_->setFriction(friction);
}

float kl::entity::get_friction() const
{
    return physics_body_->getFriction();
}

void kl::entity::set_velocity(const float3& velocity)
{
    physics_body_->setLinearVelocity({ velocity.x, velocity.y, velocity.z });
}

kl::float3 kl::entity::get_velocity() const
{
    const btVector3 velocity = physics_body_->getLinearVelocity();
    return { velocity.x(), velocity.y(), velocity.z() };
}

void kl::entity::set_angular(const float3& angular)
{
    physics_body_->setAngularVelocity({ angular.x, angular.y, angular.z });
}

kl::float3 kl::entity::get_angular() const
{
    const btVector3 angular = physics_body_->getAngularVelocity();
    return { angular.x(), angular.y(), angular.z() };
}

void kl::entity::clear_gravity()
{
    set_gravity({});
}

void kl::entity::set_gravity(const float3& gravity)
{
    physics_body_->setGravity({ gravity.x, gravity.y, gravity.z });
}

kl::float3 kl::entity::get_gravity() const
{
    const btVector3 gravity = physics_body_->getGravity();
    return { gravity.x(), gravity.y(), gravity.z() };
}

// Collision
void kl::entity::set_collider(kl::ref<btCollisionShape> collider)
{
    collider_shape_ = collider;
    physics_body_->setCollisionShape(&*collider_shape_);
}

kl::ref<btCollisionShape> kl::entity::get_collider() const
{
    return collider_shape_;
}

// Graphics
kl::mat4 kl::entity::matrix() const
{
    return mat4::translation(get_position()) * mat4::rotation(get_rotation()) * mat4::scaling(get_scale());
}

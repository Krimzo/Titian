#include "render/entity/entity.h"

#include "utility/console.h"


kl::entity::entity(PxPhysics* physics, bool dynamic)
    : is_dynamic_(dynamic)
{
    PxTransform transform = {};
    transform.q = PxQuat(PxIdentity);
    transform.p = PxVec3(PxIdentity);

    if (dynamic) {
        physics_actor_ = physics->createRigidDynamic(transform);
    }
    else {
        physics_actor_ = physics->createRigidStatic(transform);
    }

    error_check(!physics_actor_, "Failed to create physics actor");
}

kl::entity::~entity()
{
    physics_actor_->release();
}

PxRigidActor* kl::entity::get_actor() const
{
    return physics_actor_;
}

// Geometry
void kl::entity::set_rotation(const float3& rotation)
{
    const float4 quat = math::to_quaternion(rotation);

    PxTransform transform = physics_actor_->getGlobalPose();
    transform.q = *(PxQuat*)&quat;
    physics_actor_->setGlobalPose(transform);
}

kl::float3 kl::entity::get_rotation() const
{
    const PxTransform transform = physics_actor_->getGlobalPose();
    return math::to_euler(*(float4*)&transform.q);
}

void kl::entity::set_position(const float3& position)
{
    PxTransform transform = physics_actor_->getGlobalPose();
    transform.p = *(PxVec3*)&position;
    physics_actor_->setGlobalPose(transform);
}

kl::float3 kl::entity::get_position() const
{
    const PxTransform transform = physics_actor_->getGlobalPose();
    return *(float3*)&transform.p;
}

// Physics
void kl::entity::set_mass(float mass)
{
    if (is_dynamic_) {
        PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        actor->setMass(mass);
    }
}

float kl::entity::get_mass() const
{
    if (is_dynamic_) {
        const PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        return actor->getMass();
    }
    return 0.0f;
}

void kl::entity::set_velocity(const float3& velocity)
{
    if (is_dynamic_) {
        PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        actor->setLinearVelocity(*(PxVec3*)&velocity);
    }
}

kl::float3 kl::entity::get_velocity() const
{
    if (is_dynamic_) {
        const PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        const PxVec3 velocity = actor->getLinearVelocity();
        return *(float3*)&velocity;
    }
    return {};
}

void kl::entity::set_angular(const float3& angular)
{
    if (is_dynamic_) {
        PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        actor->setAngularVelocity(*(PxVec3*)&angular);
    }
}

kl::float3 kl::entity::get_angular() const
{
    if (is_dynamic_) {
        const PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        const PxVec3 angular = actor->getAngularVelocity();
        return *(float3*)&angular;
    }
    return {};
}

void kl::entity::set_gravity(const bool enabled)
{
    physics_actor_->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enabled);
}

bool kl::entity::get_gravity() const
{
    const PxActorFlags flags = physics_actor_->getActorFlags();
    return !flags.isSet(PxActorFlag::eDISABLE_GRAVITY);
}

// Collision
void kl::entity::set_collider(ref<collider> collider)
{
    if (collider_) {
        physics_actor_->detachShape(*collider_->get_shape());
    }

    collider_ = collider;

    if (collider_) {
        physics_actor_->attachShape(*collider_->get_shape());
    }
}

kl::ref<kl::collider> kl::entity::get_collider() const
{
    return collider_;
}

// Graphics
kl::mat4 kl::entity::matrix() const
{
    return mat4::translation(get_position()) * mat4::rotation(get_rotation()) * mat4::scaling(render_scale);
}

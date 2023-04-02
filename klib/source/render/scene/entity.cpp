#include "render/scene/entity.h"

#include "utility/utility.h"


#ifdef KL_USING_PHYSX

static uint64_t unique_index_counter = 0;

kl::entity::entity(PxPhysics* physics, bool dynamic)
    : unique_index(++unique_index_counter)
{
    PxTransform transform = {};
    transform.q = PxQuat(PxIdentity);
    transform.p = PxVec3(PxZero);
    regenerate_actor(physics, transform, dynamic);
}

kl::entity::~entity()
{
    physics_actor_->release();
}

// Get
PxRigidActor* kl::entity::get_actor() const
{
    return physics_actor_;
}

kl::float4x4 kl::entity::matrix() const
{
    return float4x4::translation(get_position()) * float4x4::rotation(get_rotation()) * float4x4::scaling(render_scale);
}

kl::float4x4 kl::entity::collider_matrix() const
{
    if (!collider_) {
        return {};
    }

    kl::float4x4 result = {};
    result = float4x4::translation(get_position());
    result *= float4x4::rotation(get_rotation());
    result *= float4x4::translation(collider_->get_offset());
    result *= float4x4::rotation(collider_->get_rotation());
    result *= collider_->scaling_matrix();
    return result;
}

// Geometry
void kl::entity::set_rotation(const float3& rotation)
{
    const float4 quat = to_quaternion(rotation);

    PxTransform transform = physics_actor_->getGlobalPose();
    transform.q = (PxQuat&) quat;
    physics_actor_->setGlobalPose(transform);
}

kl::float3 kl::entity::get_rotation() const
{
    const PxTransform transform = physics_actor_->getGlobalPose();
    return to_euler((float4&) transform.q);
}

void kl::entity::set_position(const float3& position)
{
    PxTransform transform = physics_actor_->getGlobalPose();
    transform.p = (PxVec3&) position;
    physics_actor_->setGlobalPose(transform);
}

kl::float3 kl::entity::get_position() const
{
    const PxTransform transform = physics_actor_->getGlobalPose();
    return (float3&) transform.p;
}

// Physics
void kl::entity::set_dynamic(PxPhysics* physics, const bool enabled)
{
    const bool old_dynamic = is_dynamic();
    if ((old_dynamic && enabled) || (!old_dynamic && !enabled)) {
        return;
    }

    PxTransform old_transform = physics_actor_->getGlobalPose();
    ref<collider> old_collider = collider_;

    set_collider(nullptr);
    regenerate_actor(physics, old_transform, enabled);
    set_collider(old_collider);
}

bool kl::entity::is_dynamic() const
{
    return (physics_actor_->getType() == PxActorType::Enum::eRIGID_DYNAMIC);
}

void kl::entity::set_gravity(const bool enabled)
{
    physics_actor_->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enabled);
    if (enabled) { wake_up(); }
}

bool kl::entity::has_gravity() const
{
    const PxActorFlags flags = physics_actor_->getActorFlags();
    return !flags.isSet(PxActorFlag::eDISABLE_GRAVITY);
}

void kl::entity::set_mass(float mass)
{
    if (is_dynamic()) {
        PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        actor->setMass(mass);
    }
}

float kl::entity::get_mass() const
{
    if (is_dynamic()) {
        const PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        return actor->getMass();
    }
    return 0.0f;
}

void kl::entity::set_velocity(const float3& velocity)
{
    if (is_dynamic()) {
        PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        actor->setLinearVelocity((PxVec3&) velocity);
    }
}

kl::float3 kl::entity::get_velocity() const
{
    if (is_dynamic()) {
        const PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        const PxVec3 velocity = actor->getLinearVelocity();
        return (float3&) velocity;
    }
    return {};
}

void kl::entity::set_angular(const float3& angular)
{
    if (is_dynamic()) {
        PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        actor->setAngularVelocity((PxVec3&) angular);
    }
}

kl::float3 kl::entity::get_angular() const
{
    if (is_dynamic()) {
        const PxRigidDynamic* actor = (PxRigidDynamic*) physics_actor_;
        const PxVec3 angular = actor->getAngularVelocity();
        return (float3&) angular;
    }
    return {};
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

// Private
void kl::entity::regenerate_actor(PxPhysics* physics, const PxTransform& transform, const bool dynamic)
{
    if (physics_actor_) {
        physics_actor_->release();
        physics_actor_ = nullptr;
    }

    physics_actor_ = dynamic ? (PxRigidActor*) physics->createRigidDynamic(transform) : physics->createRigidStatic(transform);
    error_check(!physics_actor_, "Failed to create physics actor");

    if (dynamic) {
        set_mass(1.0f);
        set_gravity(false);
    }
}

void kl::entity::wake_up() const
{
    if (is_dynamic()) {
        auto actor = (PxRigidDynamic*) physics_actor_;
        if (actor->getScene()) {
            actor->wakeUp();
        }
    }
}

#else

// Creation
kl::entity::entity()
{}

kl::entity::~entity()
{}

// Methods
void kl::entity::update_physics(const float delta_t)
{
    velocity += acceleration * delta_t;
    position += velocity * delta_t;
    rotation += angular * delta_t;
}

kl::float4x4 kl::entity::matrix() const
{
    return float4x4::translation(position) * float4x4::rotation(rotation) * float4x4::scaling(scale);
}

#endif

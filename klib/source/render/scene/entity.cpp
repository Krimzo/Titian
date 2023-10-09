#include "klib.h"


#ifdef KL_USING_PHYSX

static uint64_t UNIQUE_INDEX_COUNTER = 0;

kl::Entity::Entity(physx::PxPhysics* physics, const bool dynamic)
    : unique_index(++UNIQUE_INDEX_COUNTER)
{
    physx::PxTransform transform = {};
    transform.q = physx::PxQuat(physx::PxIdentity);
    transform.p = physx::PxVec3(physx::PxZero);
    generate_actor(physics, transform, dynamic);
}

kl::Entity::~Entity()
{
    m_physics_actor->release();
}

// Get
physx::PxRigidActor* kl::Entity::actor() const
{
    return m_physics_actor;
}

kl::Float4x4 kl::Entity::matrix() const
{
    return Float4x4::translation(position()) * Float4x4::rotation(rotation()) * Float4x4::scaling(render_scale);
}

kl::Float4x4 kl::Entity::collider_matrix() const
{
    if (!m_collider) {
        return {};
    }

    kl::Float4x4 result = Float4x4::translation(position());
    result *= Float4x4::rotation(rotation());
    result *= Float4x4::translation(m_collider->offset());
    result *= Float4x4::rotation(m_collider->rotation());
    result *= m_collider->scaling_matrix();
    return result;
}

// Geometry
void kl::Entity::set_rotation(const Float3& rotation)
{
    const Float4 quat = to_quaternion(rotation);
    physx::PxTransform transform = m_physics_actor->getGlobalPose();
    transform.q = (const physx::PxQuat&) quat;
    m_physics_actor->setGlobalPose(transform);
}

kl::Float3 kl::Entity::rotation() const
{
    const physx::PxTransform transform = m_physics_actor->getGlobalPose();
    return to_euler((const Float4&) transform.q);
}

void kl::Entity::set_position(const Float3& position)
{
    physx::PxTransform transform = m_physics_actor->getGlobalPose();
    transform.p = (const physx::PxVec3&) position;
    m_physics_actor->setGlobalPose(transform);
}

kl::Float3 kl::Entity::position() const
{
    const physx::PxTransform transform = m_physics_actor->getGlobalPose();
    return (const Float3&) transform.p;
}

// Physics
void kl::Entity::set_dynamic(physx::PxPhysics* physics, const bool enabled)
{
    const bool old_dynamic = is_dynamic();
    if ((old_dynamic && enabled) || (!old_dynamic && !enabled)) {
        return;
    }

    const physx::PxTransform old_transform = m_physics_actor->getGlobalPose();
    const Object<Collider> old_collider = m_collider;

    set_collider(nullptr);
    generate_actor(physics, old_transform, enabled);
    set_collider(old_collider);
}

bool kl::Entity::is_dynamic() const
{
    return m_physics_actor->getType() == physx::PxActorType::Enum::eRIGID_DYNAMIC;
}

void kl::Entity::set_gravity(const bool enabled)
{
    m_physics_actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enabled);
    if (enabled) { wake_up(); }
}

bool kl::Entity::has_gravity() const
{
    const physx::PxActorFlags flags = m_physics_actor->getActorFlags();
    return !flags.isSet(physx::PxActorFlag::eDISABLE_GRAVITY);
}

void kl::Entity::set_mass(const float mass)
{
    if (is_dynamic()) {
        physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
        actor->setMass(mass);
    }
}

float kl::Entity::mass() const
{
    if (is_dynamic()) {
        const physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
        return actor->getMass();
    }
    return 0.0f;
}

void kl::Entity::set_velocity(const Float3& velocity)
{
    if (is_dynamic()) {
        physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
        actor->setLinearVelocity((const physx::PxVec3&) velocity);
    }
}

kl::Float3 kl::Entity::velocity() const
{
    if (is_dynamic()) {
        const physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
        const physx::PxVec3 velocity = actor->getLinearVelocity();
        return (const Float3&) velocity;
    }
    return {};
}

void kl::Entity::set_angular(const Float3& angular)
{
    if (is_dynamic()) {
        physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
        actor->setAngularVelocity((const physx::PxVec3&) angular);
    }
}

kl::Float3 kl::Entity::angular() const
{
    if (is_dynamic()) {
        const physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
        const physx::PxVec3 angular = actor->getAngularVelocity();
        return (const Float3&) angular;
    }
    return {};
}

// Collision
void kl::Entity::set_collider(const Object<kl::Collider>& collider)
{
    if (m_collider) {
        m_physics_actor->detachShape(*m_collider->shape());
    }

    m_collider = collider;

    if (m_collider) {
        m_physics_actor->attachShape(*m_collider->shape());
    }
}

kl::Object<kl::Collider> kl::Entity::collider() const
{
}

// Private
void kl::Entity::generate_actor(physx::PxPhysics* physics, const physx::PxTransform& transform, const bool dynamic)
{
    if (m_physics_actor) {
        m_physics_actor->release();
        m_physics_actor = nullptr;
    }

    m_physics_actor = dynamic ? (physx::PxRigidActor*) physics->createRigidDynamic(transform) : physics->createRigidStatic(transform);
    assert(m_physics_actor, "Failed to create physics actor");

    if (dynamic) {
        set_mass(1.0f);
        set_gravity(false);
    }
}

void kl::Entity::wake_up() const
{
    if (is_dynamic()) {
        physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*) m_physics_actor;
        if (actor->getScene()) {
            actor->wakeUp();
        }
    }
}

#else

void kl::Entity::update_physics(const float delta_t)
{
    velocity += acceleration * delta_t;
    position += velocity * delta_t;
    rotation += angular * delta_t;
}

kl::Float4x4 kl::Entity::matrix() const
{
    return Float4x4::translation(position) * Float4x4::rotation(rotation) * Float4x4::scaling(scale);
}

#endif

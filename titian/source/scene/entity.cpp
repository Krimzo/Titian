#include "titian.h"


titian::Entity::Entity(px::PxPhysics* physics, const bool dynamic)
    : m_physics(physics)
{
    px::PxTransform transform = {};
    transform.q = px::PxQuat(px::PxIdentity);
    transform.p = px::PxVec3(px::PxZero);
    generate_actor(transform, dynamic);
}

titian::Entity::~Entity()
{
    cleanup();
}

void titian::Entity::serialize(Serializer* serializer, const void* helper_data) const
{
    serializer->write_string("entity_type", typeid(self).name()); // must be read from outside

    serializer->write_bool("is_dynamic", is_dynamic());
    serializer->write_bool("has_gravity", has_gravity());
    serializer->write_bool("casts_shadows", casts_shadows);

    serializer->write_float("mass", mass());

    serializer->write_float_array("scale", scale, 3);
    serializer->write_float_array("rotation", rotation(), 3);
    serializer->write_float_array("position", position(), 3);
    serializer->write_float_array("velocity", velocity(), 3);
    serializer->write_float_array("angular", angular(), 3);

    serializer->write_string("animation_name", animation_name);
    serializer->write_string("material_name", material_name);
    serializer->write_string("collider_mesh_name", collider_mesh_name);

    const bool has_collider = m_collider;
    serializer->write_bool("has_collider", has_collider);
    if (has_collider) {
        serializer->push_object("collider");
        m_collider->serialize(serializer, helper_data);
        serializer->pop_object();
    }
}

void titian::Entity::deserialize(const Serializer* serializer, const void* helper_data)
{
    bool is_dynamic = false;
    serializer->read_bool("is_dynamic", is_dynamic);
    set_dynamic(is_dynamic);

    bool has_gravity = false;
    serializer->read_bool("has_gravity", has_gravity);
    set_gravity(has_gravity);

    serializer->read_bool("casts_shadows", casts_shadows);
    
    float mass = 0.0f;
    serializer->read_float("mass", mass);
    set_mass(mass);

    serializer->read_float_array("scale", scale, 3);

    Float3 rotation;
    serializer->read_float_array("rotation", rotation, 3);
    set_rotation(rotation);

    Float3 position;
    serializer->read_float_array("position", position, 3);
    set_position(position);

    Float3 velocity;
    serializer->read_float_array("velocity", velocity, 3);
    set_velocity(velocity);

    Float3 angular;
    serializer->read_float_array("angular", angular, 3);
    set_angular(angular);

    serializer->read_string("animation_name", animation_name);
    serializer->read_string("material_name", material_name);
    serializer->read_string("collider_mesh_name", collider_mesh_name);

    bool has_collider = false;
    serializer->read_bool("has_collider", has_collider);
    if (has_collider) {
        Ref new_collider = new Collider(m_physics);
        serializer->load_object("collider");
        new_collider->deserialize(serializer, helper_data);
        serializer->unload_object();
        this->set_collider(new_collider);
    }
}

// Get
px::PxRigidActor* titian::Entity::actor() const
{
    return m_actor;
}

titian::Float4x4 titian::Entity::model_matrix() const
{
    return Float4x4::translation(position()) * Float4x4::rotation(rotation()) * Float4x4::scaling(scale);
}

titian::Float4x4 titian::Entity::collider_matrix() const
{
    if (!m_collider) {
        return {};
    }

    Float4x4 result = Float4x4::translation(position());
    result *= Float4x4::rotation(rotation());
    result *= Float4x4::translation(m_collider->offset());
    result *= Float4x4::rotation(m_collider->rotation());
    result *= m_collider->scaling_matrix();
    return result;
}

// Geometry
void titian::Entity::set_rotation(const Float3& rotation)
{
    const Float4 quat = kl::to_quaternion(rotation);
    px::PxTransform transform = m_actor->getGlobalPose();
    transform.q = (const px::PxQuat&)quat;
    m_actor->setGlobalPose(transform);
}

titian::Float3 titian::Entity::rotation() const
{
    const px::PxTransform transform = m_actor->getGlobalPose();
    return kl::to_euler<float>((const Float4&) transform.q);
}

void titian::Entity::set_position(const Float3& position)
{
    px::PxTransform transform = m_actor->getGlobalPose();
    transform.p = (const px::PxVec3&)position;
    m_actor->setGlobalPose(transform);
}

titian::Float3 titian::Entity::position() const
{
    const px::PxTransform transform = m_actor->getGlobalPose();
    return (const Float3&)transform.p;
}

// Physics
void titian::Entity::set_dynamic(const bool enabled)
{
    const bool old_dynamic = is_dynamic();
    if ((old_dynamic && enabled) || (!old_dynamic && !enabled)) {
        return;
    }
    const px::PxTransform old_transform = m_actor->getGlobalPose();
    Ref<Collider> old_collider = m_collider;
    set_collider(nullptr);
    generate_actor(old_transform, enabled);
    set_collider(old_collider);
}

bool titian::Entity::is_dynamic() const
{
    const px::PxActorType::Enum type = m_actor->getType();
    return type == px::PxActorType::Enum::eRIGID_DYNAMIC;
}

void titian::Entity::set_gravity(const bool enabled)
{
    m_actor->setActorFlag(px::PxActorFlag::eDISABLE_GRAVITY, !enabled);
    if (enabled) {
        wake_up();
    }
}

bool titian::Entity::has_gravity() const
{
    const px::PxActorFlags flags = m_actor->getActorFlags();
    return !flags.isSet(px::PxActorFlag::eDISABLE_GRAVITY);
}

void titian::Entity::set_mass(const float mass)
{
    if (!is_dynamic()) {
        return;
    }
    px::PxRigidDynamic* actor = (px::PxRigidDynamic*)m_actor;
    actor->setMass(mass);
}

float titian::Entity::mass() const
{
    if (!is_dynamic()) {
        return 0.0f;
    }
    const px::PxRigidDynamic* actor = (px::PxRigidDynamic*)m_actor;
    return actor->getMass();
}

void titian::Entity::set_velocity(const Float3& velocity)
{
    if (!is_dynamic()) {
        return;
    }
    px::PxRigidDynamic* actor = (px::PxRigidDynamic*)m_actor;
    actor->setLinearVelocity((const px::PxVec3&)velocity);
}

titian::Float3 titian::Entity::velocity() const
{
    if (!is_dynamic()) {
        return {};
    }
    const px::PxRigidDynamic* actor = (px::PxRigidDynamic*)m_actor;
    const px::PxVec3 velocity = actor->getLinearVelocity();
    return (const Float3&)velocity;
}

void titian::Entity::set_angular(const Float3& angular)
{
    if (!is_dynamic()) {
        return;
    }
    px::PxRigidDynamic* actor = (px::PxRigidDynamic*)m_actor;
    actor->setAngularVelocity((const px::PxVec3&)angular);
}

titian::Float3 titian::Entity::angular() const
{
    if (!is_dynamic()) {
        return {};
    }
    const px::PxRigidDynamic* actor = (px::PxRigidDynamic*)m_actor;
    const px::PxVec3 angular = actor->getAngularVelocity();
    return (const Float3&) angular;
}

// Collision
void titian::Entity::set_collider(const Ref<Collider>& collider)
{
    if (m_collider) {
        m_actor->detachShape(*m_collider->shape());
    }
    if (collider) {
        m_actor->attachShape(*collider->shape());
    }
    m_collider = collider;
}

titian::Ref<titian::Collider> titian::Entity::collider() const
{
    return m_collider;
}

void titian::Entity::cleanup()
{
    if (m_actor) {
        m_actor->release();
        m_actor = nullptr;
    }
}

void titian::Entity::generate_actor(const px::PxTransform& transform, const bool dynamic)
{
    cleanup();
    if (dynamic) {
        m_actor = m_physics->createRigidDynamic(transform);
        set_mass(1.0f);
        set_gravity(false);
    }
    else {
        m_actor = m_physics->createRigidStatic(transform);
    }
    kl::assert(m_actor, "Failed to create physics actor");
    m_actor->userData = this;
}

void titian::Entity::wake_up() const
{
    if (!is_dynamic()) {
        return;
    }
    px::PxRigidDynamic* actor = (px::PxRigidDynamic*)m_actor;
    if (actor->getScene()) {
        actor->wakeUp();
    }
}

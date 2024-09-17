#include "titian.h"


titian::Entity::Entity(px::PxPhysics* physics)
    : m_physics(physics)
{
    if (!m_physics)
        return;

    px::PxTransform transform{};
    transform.q = px::PxQuat(px::PxIdentity);
    transform.p = px::PxVec3(px::PxZero);
    generate_actor(transform, false);
}

titian::Entity::~Entity()
{
    if (m_actor) m_actor->release();
}

void titian::Entity::serialize(Serializer* serializer, const void* helper_data) const
{
    serializer->write_string("entity_type", typeid(*this).name());

    serializer->write_bool("casts_shadows", casts_shadows);

    serializer->write_string("animation_name", animation_name);
    serializer->write_string("material_name", material_name);
    serializer->write_string("collider_mesh_name", collider_mesh_name);

    serializer->write_bool("dynamic", dynamic());
    serializer->write_bool("gravity", gravity());
    serializer->write_float("mass", mass());

    const Float3 scale = this->scale();
    serializer->write_float_array("scale", &scale.x, 3);

    const Float3 rotation = this->rotation();
    serializer->write_float_array("rotation", &rotation.x, 3);

    const Float3 position = this->position();
    serializer->write_float_array("position", &position.x, 3);

    const Float3 velocity = this->velocity();
    serializer->write_float_array("velocity", &velocity.x, 3);

    const Float3 angular = this->angular();
    serializer->write_float_array("angular", &angular.x, 3);

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
    serializer->read_bool("casts_shadows", casts_shadows);

    serializer->read_string("animation_name", animation_name);
    serializer->read_string("material_name", material_name);
    serializer->read_string("collider_mesh_name", collider_mesh_name);

    bool dynamic = false;
    serializer->read_bool("dynamic", dynamic);
    set_dynamic(dynamic);

    bool gravity = false;
    serializer->read_bool("gravity", gravity);
    set_gravity(gravity);
    
    float mass = 0.0f;
    serializer->read_float("mass", mass);
    set_mass(mass);

	Float3 scale;
    serializer->read_float_array("scale", &scale.x, 3);
	set_scale(scale);

    Float3 rotation;
    serializer->read_float_array("rotation", &rotation.x, 3);
    set_rotation(rotation);

    Float3 position;
    serializer->read_float_array("position", &position.x, 3);
    set_position(position);

    Float3 velocity;
    serializer->read_float_array("velocity", &velocity.x, 3);
    set_velocity(velocity);

    Float3 angular;
    serializer->read_float_array("angular", &angular.x, 3);
    set_angular(angular);

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

titian::EntityType titian::Entity::type() const
{
    if (!m_physics)
        return EntityType::VIRTUAL;
    if (m_actor->getType() != px::PxActorType::Enum::eRIGID_DYNAMIC)
        return EntityType::STATIC;
    return EntityType::DYNAMIC;
}

px::PxRigidActor* titian::Entity::actor() const
{
    return m_actor;
}

void titian::Entity::set_dynamic(const bool enabled)
{
    if (type() == EntityType::VIRTUAL)
		return;

    const px::PxTransform old_transform = m_actor->getGlobalPose();
    Ref<Collider> old_collider = m_collider;
    set_collider(nullptr);
    generate_actor(old_transform, enabled);
    set_collider(old_collider);
}

bool titian::Entity::dynamic() const
{
    return type() == EntityType::DYNAMIC;
}

void titian::Entity::set_gravity(const bool enabled)
{
    if (type() != EntityType::DYNAMIC)
        return;

    m_actor->setActorFlag(px::PxActorFlag::eDISABLE_GRAVITY, !enabled);
    if (!enabled)
        return;

    px::PxRigidDynamic* actor = (px::PxRigidDynamic*) m_actor;
    if (actor->getScene()) {
        actor->wakeUp();
    }
}

bool titian::Entity::gravity() const
{
    if (type() != EntityType::DYNAMIC)
        return false;

    const px::PxActorFlags flags = m_actor->getActorFlags();
    return !flags.isSet(px::PxActorFlag::eDISABLE_GRAVITY);
}

void titian::Entity::set_mass(const float mass)
{
    if (type() != EntityType::DYNAMIC)
        return;

    px::PxRigidDynamic* actor = (px::PxRigidDynamic*) m_actor;
    actor->setMass(mass);
}

float titian::Entity::mass() const
{
    if (type() != EntityType::DYNAMIC)
        return 0.0f;

    const px::PxRigidDynamic* actor = (px::PxRigidDynamic*) m_actor;
    return actor->getMass();
}

void titian::Entity::set_angular_damping(float damping)
{
	if (type() != EntityType::DYNAMIC)
		return;

	px::PxRigidDynamic* actor = (px::PxRigidDynamic*) m_actor;
	actor->setAngularDamping(damping);
}

float titian::Entity::angular_damping() const
{
	if (type() != EntityType::DYNAMIC)
		return 0.0f;

	const px::PxRigidDynamic* actor = (px::PxRigidDynamic*) m_actor;
	return actor->getAngularDamping();
}

void titian::Entity::set_scale(const Float3& scale)
{
    m_scale = scale;
}

titian::Float3 titian::Entity::scale() const
{
	return m_scale;
}

void titian::Entity::set_rotation(const Float3& rotation)
{
    if (type() == EntityType::VIRTUAL) {
        m_rotation = rotation;
        return;
    }

    px::PxTransform transform = m_actor->getGlobalPose();
    transform.q = px_cast(Float4{ kl::to_quaternion(rotation) });
    m_actor->setGlobalPose(transform);
}

titian::Float3 titian::Entity::rotation() const
{
	if (type() == EntityType::VIRTUAL)
		return m_rotation;

    const px::PxTransform transform = m_actor->getGlobalPose();
    return kl::to_euler(Quaternion{ px_cast(transform.q) });
}

void titian::Entity::set_position(const Float3& position)
{
	if (type() == EntityType::VIRTUAL) {
		m_position = position;
		return;
	}

    px::PxTransform transform = m_actor->getGlobalPose();
    transform.p = px_cast(position);
    m_actor->setGlobalPose(transform);
}

titian::Float3 titian::Entity::position() const
{
	if (type() == EntityType::VIRTUAL)
		return m_position;

    const px::PxTransform transform = m_actor->getGlobalPose();
    return px_cast(transform.p);
}

void titian::Entity::set_velocity(const Float3& velocity)
{
    if (type() != EntityType::DYNAMIC)
        return;

    px::PxRigidDynamic* actor = (px::PxRigidDynamic*) m_actor;
    actor->setLinearVelocity(px_cast(velocity));
}

titian::Float3 titian::Entity::velocity() const
{
    if (type() != EntityType::DYNAMIC)
        return {};

    const px::PxRigidDynamic* actor = (px::PxRigidDynamic*) m_actor;
    const px::PxVec3 velocity = actor->getLinearVelocity();
    return px_cast(velocity);
}

void titian::Entity::set_angular(const Float3& angular)
{
    if (type() != EntityType::DYNAMIC)
        return;

    px::PxRigidDynamic* actor = (px::PxRigidDynamic*) m_actor;
    actor->setAngularVelocity(px_cast(angular));
}

titian::Float3 titian::Entity::angular() const
{
    if (type() != EntityType::DYNAMIC)
        return {};

    const px::PxRigidDynamic* actor = (px::PxRigidDynamic*) m_actor;
    const px::PxVec3 angular = actor->getAngularVelocity();
    return px_cast(angular);
}

void titian::Entity::set_collider(const Ref<Collider>& collider)
{
    if (type() == EntityType::VIRTUAL)
        return;

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

titian::Float4x4 titian::Entity::model_matrix() const
{
    return Float4x4::translation(position())
        * Float4x4::rotation(rotation())
        * Float4x4::scaling(scale());
}

titian::Float4x4 titian::Entity::collider_matrix() const
{
    if (!m_collider)
        return {};
    return Float4x4::translation(position())
        * Float4x4::rotation(rotation())
        * Float4x4::translation(m_collider->offset())
        * Float4x4::rotation(m_collider->rotation())
        * m_collider->scaling_matrix();
}

titian::Ref<titian::Entity> titian::Entity::clone() const
{
    Ref entity = new Entity(m_physics);
    entity->casts_shadows = casts_shadows;
    entity->animation_name = animation_name;
    entity->material_name = material_name;
    entity->collider_mesh_name = collider_mesh_name;
    entity->set_dynamic(dynamic());
    entity->set_gravity(gravity());
    entity->set_mass(mass());
    entity->set_angular_damping(angular_damping());
    entity->set_scale(scale());
    entity->set_rotation(rotation());
    entity->set_position(position());
    entity->set_velocity(velocity());
    entity->set_angular(angular());
    if (auto collider = this->collider()) {
        entity->set_collider(collider->clone());
    }
    return entity;
}

void titian::Entity::generate_actor(const px::PxTransform& transform, const bool dynamic)
{
    if (m_actor)
        m_actor->release();

    if (dynamic) {
        m_actor = m_physics->createRigidDynamic(transform);
    }
    else {
        m_actor = m_physics->createRigidStatic(transform);
    }
    kl::assert(m_actor, "Failed to create physics actor");

    m_actor->userData = this;
    set_gravity(false);
    set_mass(1.0f);
    set_angular_damping(0.0f);
}

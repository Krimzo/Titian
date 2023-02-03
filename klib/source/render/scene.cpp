#include "render/scene.h"


kl::scene::scene()
{
	configuration_ = make<btDefaultCollisionConfiguration>();
	dispatcher_ = make<btCollisionDispatcher>(&*configuration_);
	pair_cache_ = make<btDbvtBroadphase>();
	solver_ = make<btSequentialImpulseConstraintSolver>();
	world_ = make<btDiscreteDynamicsWorld>(&*dispatcher_, &*pair_cache_, &*solver_, &*configuration_);

	set_gravity({ 0.0f, -9.81f, 0.0f });
}

kl::scene::~scene()
{
	world_ = {};
	solver_ = {};
	pair_cache_ = {};
	dispatcher_ = {};
	configuration_ = {};
}

void kl::scene::set_gravity(const float3& gravity)
{
	world_->setGravity({ gravity.x, gravity.y, gravity.z });
}

kl::float3 kl::scene::get_gravity() const
{
	const btVector3 gravity = world_->getGravity();
	return { gravity.x(), gravity.y(), gravity.z() };
}

std::set<kl::ref<kl::entity>>::iterator kl::scene::begin()
{
	return entities_.begin();
}

std::set<kl::ref<kl::entity>>::iterator kl::scene::end()
{
	return entities_.end();
}

void kl::scene::add(ref<entity> entity)
{
	entities_.insert(entity);
	world_->addRigidBody(entity->get_body());
}

void kl::scene::remove(ref<entity> entity)
{
	entities_.erase(entity);
	world_->removeRigidBody(entity->get_body());
}

void kl::scene::update_physics(float delta_t)
{
	world_->applyGravity();
	world_->stepSimulation(delta_t);
}

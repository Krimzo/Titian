#include "physics/physics_scene.h"


physics_scene::physics_scene()
{
	configuration_ = kl::make<btDefaultCollisionConfiguration>();
	dispatcher_ = kl::make<btCollisionDispatcher>(&*configuration_);
	pair_cache_ = kl::make<btDbvtBroadphase>();
	solver_ = kl::make<btSequentialImpulseConstraintSolver>();
	world_ = kl::make<btDiscreteDynamicsWorld>(&*dispatcher_, &*pair_cache_, &*solver_, &*configuration_);

	world_->setGravity(btVector3(0.0f, -9.81f, 0.0f));
}

physics_scene::~physics_scene()
{
	world_ = {};
	solver_ = {};
	pair_cache_ = {};
	dispatcher_ = {};
	configuration_ = {};
}

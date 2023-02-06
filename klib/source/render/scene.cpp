#include "render/scene.h"

#include "utility/console.h"


#ifdef KL_USING_PHYSX

PxDefaultAllocator kl::scene::allocator_ = {};

PxDefaultErrorCallback kl::scene::error_callback_ = {};

PxFoundation* kl::scene::foundation_ = PxCreateFoundation(PX_PHYSICS_VERSION, allocator_, error_callback_);

kl::scene::scene()
{
	physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation_, PxTolerancesScale());
	error_check(!physics_, "Failed to create physics");

	cooking_ = PxCreateCooking(PX_PHYSICS_VERSION, *foundation_, PxCookingParams(physics_->getTolerancesScale()));
	error_check(!cooking_, "Failed to create physics cooking");

	dispatcher_ = PxDefaultCpuDispatcherCreate(2);
	error_check(!dispatcher_, "Failed to create physics dispatcher_");

	PxSceneDesc scene_descriptor = { physics_->getTolerancesScale() };
	scene_descriptor.gravity.y = -9.81f;
	scene_descriptor.cpuDispatcher = dispatcher_;
	scene_descriptor.filterShader = PxDefaultSimulationFilterShader;

	scene_ = physics_->createScene(scene_descriptor);
	error_check(!scene_, "Failed to create physics scene");
}

kl::scene::~scene()
{
	while (!entities_.empty()) {
		this->remove(*entities_.begin());
	}

	     scene_->release();
	dispatcher_->release();
	   cooking_->release();
	   physics_->release();
}

// Scene properties
void kl::scene::set_gravity(const float3& gravity)
{
	scene_->setGravity({ gravity.x, gravity.y, gravity.z });
}

kl::float3 kl::scene::get_gravity() const
{
	const PxVec3 gravity = scene_->getGravity();
	return { gravity.x, gravity.y, gravity.z };
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
	scene_->addActor(*entity->get_actor());
}

void kl::scene::remove(ref<entity> entity)
{
	scene_->removeActor(*entity->get_actor());
	entities_.erase(entity);
}

int kl::scene::entity_count() const
{
	return (int) entities_.size();
}

void kl::scene::update_physics(float delta_t)
{
	scene_->simulate(delta_t);
	scene_->fetchResults(true);
}

// Entity
kl::ref<kl::entity> kl::scene::make_entity(bool dynamic)
{
	return make<entity>(physics_, dynamic);
}

// Dynamic colliders
kl::ref<kl::collider> kl::scene::make_box_collider(const float3& scale)
{
	return make<collider>(physics_, PxBoxGeometry(*(PxVec3*)&scale));
}

kl::ref<kl::collider> kl::scene::make_sphere_collider(float radius)
{
	return make<collider>(physics_, PxSphereGeometry(radius));
}

kl::ref<kl::collider> kl::scene::make_capsule_collider(float radius, float height)
{
	return make<collider>(physics_, PxCapsuleGeometry(radius, height));
}

// Static colliders
kl::ref<kl::collider> kl::scene::make_plane_collider()
{
	return make<collider>(physics_, PxPlaneGeometry());
}

kl::ref<kl::collider> kl::scene::make_mesh_collider(const mesh_data& mesh_data, const float3& scale)
{
	return nullptr;
}

#else

kl::scene::scene()
{}

kl::scene::~scene()
{}

void kl::scene::update_physics(const float delta_t)
{
	for (auto& entity : *this) {
		entity->velocity += gravity * delta_t;
		entity->velocity += entity->acceleration * delta_t;
		entity->position += entity->velocity * delta_t;
		entity->rotation += entity->angular * delta_t;
	}
}

#endif

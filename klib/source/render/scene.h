#pragma once

#include "render/entity.h"
#include "render/camera.h"
#include "memory/ref.h"

#include <set>


namespace kl {
	class scene 
	{
		std::set<kl::ref<kl::entity>> entities_;

		kl::ref<btDefaultCollisionConfiguration>    configuration_ = {};
		kl::ref<btCollisionDispatcher>                 dispatcher_ = {};
		kl::ref<btBroadphaseInterface>                 pair_cache_ = {};
		kl::ref<btSequentialImpulseConstraintSolver>       solver_ = {};
		kl::ref<btDiscreteDynamicsWorld>                    world_ = {};

	public:
		kl::camera camera = {};

		float ambient_light = 0.1f;
		kl::float3 sun_direction = {};

		kl::dx::buffer ocean_mesh = nullptr;

		scene();
		~scene();

		scene(const scene&) = delete;
		scene(const scene&&) = delete;

		void operator=(const scene&) = delete;
		void operator=(const scene&&) = delete;

		void set_gravity(const float3& gravity);
		float3 get_gravity() const;

		std::set<kl::ref<kl::entity>>::iterator begin();
		std::set<kl::ref<kl::entity>>::iterator end();

		void add(kl::ref<entity> entity);
		void remove(kl::ref<entity> entity);

		void update_physics(float delta_t);

		kl::mat4 sun_matrix();
	};
}

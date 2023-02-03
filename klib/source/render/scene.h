#pragma once

#include "render/entity.h"
#include "render/camera.h"

#include "render/light/ambient_light.h"
#include "render/light/directional_light.h"

#include "memory/ref.h"
#include <set>


namespace kl {
	class scene 
	{
		std::set<ref<entity>> entities_;

		ref<btDefaultCollisionConfiguration>    configuration_ = {};
		ref<btCollisionDispatcher>                 dispatcher_ = {};
		ref<btBroadphaseInterface>                 pair_cache_ = {};
		ref<btSequentialImpulseConstraintSolver>       solver_ = {};
		ref<btDiscreteDynamicsWorld>                    world_ = {};

	public:
		camera camera = {};

		ref<ambient_light>         ambient_light = {};
		ref<directional_light> directional_light = {};

		dx::buffer ocean_mesh = nullptr;

		scene();
		~scene();

		scene(const scene&) = delete;
		scene(const scene&&) = delete;

		void operator=(const scene&) = delete;
		void operator=(const scene&&) = delete;

		void set_gravity(const float3& gravity);
		float3 get_gravity() const;

		std::set<ref<entity>>::iterator begin();
		std::set<ref<entity>>::iterator end();

		void add(ref<entity> entity);
		void remove(ref<entity> entity);

		void update_physics(float delta_t);
	};
}

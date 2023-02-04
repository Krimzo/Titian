#pragma once

#include "render/camera.h"

#include "render/entity/entity.h"

#include "render/light/ambient_light.h"
#include "render/light/directional_light.h"

#include "memory/ref.h"

#include <set>


namespace kl {
	class scene 
	{
		std::set<ref<entity>> entities_;

		PxDefaultAllocator          allocator_ = {};
		PxDefaultErrorCallback error_callback_ = {};

		PxFoundation*           foundation_ = nullptr;
		PxPhysics*                 physics_ = nullptr;
		PxCooking*                 cooking_ = nullptr;
		PxDefaultCpuDispatcher* dispatcher_ = nullptr;
		PxScene*                     scene_ = nullptr;

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

		// Scene properties
		void set_gravity(const float3& gravity);
		float3 get_gravity() const;

		std::set<ref<entity>>::iterator begin();
		std::set<ref<entity>>::iterator end();

		void add(ref<entity> entity);
		void remove(ref<entity> entity);

		int entity_count() const;

		void update_physics(float delta_t);

		// Entity
		ref<entity> make_entity(bool dynamic);

		// Dynamic colliders
		ref<collider> make_box_collider(const float3& scale);
		ref<collider> make_sphere_collider(float radius);
		ref<collider> make_capsule_collider(float radius, float height);

		// Static colliders
		ref<collider> make_plane_collider();
		ref<collider> make_mesh_collider(const mesh_data& mesh_data, const float3& scale);
	};
}

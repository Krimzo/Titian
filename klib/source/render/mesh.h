#pragma once

#include "graphics/dx_types.h"
#include "math/ray_casting/vertex.h"
#include "memory/ref.h"

#include "btBulletDynamicsCommon.h"

#include <vector>


namespace kl {
	using mesh = dx::buffer;
	using mesh_data = std::vector<vertex>;

	kl::ref<btCollisionShape> make_empty_collider();

	kl::ref<btCollisionShape> make_box_collider(const kl::float3& scale = kl::float3::splash(1.0f));

	kl::ref<btCollisionShape> make_sphere_collider(float radius);

	kl::ref<btCollisionShape> make_sphere_collider(const mesh_data& data);

	kl::ref<btCollisionShape> make_capsule_collider(float radius, float height);

	kl::ref<btCollisionShape> make_static_mesh_collider(const mesh_data& data);
}

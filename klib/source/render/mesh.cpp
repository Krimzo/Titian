#include "render/mesh.h"


kl::ref<btCollisionShape> kl::make_empty_collider()
{
	return make<btEmptyShape>();
}

kl::ref<btCollisionShape> kl::make_box_collider(const kl::float3& scale)
{
	return make<btBoxShape>(btVector3(scale.x, scale.y, scale.z));
}

kl::ref<btCollisionShape> kl::make_sphere_collider(const float radius)
{
	return make<btSphereShape>(radius);
}

kl::ref<btCollisionShape> kl::make_sphere_collider(const mesh_data& data)
{
	float max_length = 0.0f;
	for (auto& vertex : data) {
		max_length = std::max(max_length, vertex.world.length());
	}
	return make_sphere_collider(max_length);
}

kl::ref<btCollisionShape> kl::make_capsule_collider(const float radius, const float height)
{
	return make<btCapsuleShape>(radius, height);
}

kl::ref<btCollisionShape> kl::make_static_mesh_collider(const mesh_data& data)
{
	btTriangleMesh triangle_mesh = {};

	for (size_t i = 0; i < data.size(); i += 3) {
		const btVector3 a = { data[i + 0].world.x, data[i + 0].world.y, data[i + 0].world.z };
		const btVector3 b = { data[i + 1].world.x, data[i + 1].world.y, data[i + 1].world.z };
		const btVector3 c = { data[i + 2].world.x, data[i + 2].world.y, data[i + 2].world.z };
		triangle_mesh.addTriangle(a, b, c);
	}

	return make<btBvhTriangleMeshShape>(&triangle_mesh, true);
}

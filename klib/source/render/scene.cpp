#include "render/scene.h"


kl::scene::scene()
{
	configuration_ = kl::make<btDefaultCollisionConfiguration>();
	dispatcher_ = kl::make<btCollisionDispatcher>(&*configuration_);
	pair_cache_ = kl::make<btDbvtBroadphase>();
	solver_ = kl::make<btSequentialImpulseConstraintSolver>();
	world_ = kl::make<btDiscreteDynamicsWorld>(&*dispatcher_, &*pair_cache_, &*solver_, &*configuration_);

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

void kl::scene::add(kl::ref<entity> entity)
{
	entities_.insert(entity);
	world_->addRigidBody(entity->get_body());
}

void kl::scene::remove(kl::ref<entity> entity)
{
	entities_.erase(entity);
	world_->removeRigidBody(entity->get_body());
}

void kl::scene::update_physics(float delta_t)
{
	world_->applyGravity();
	world_->stepSimulation(delta_t);
}

kl::mat4 kl::scene::sun_matrix()
{
	sun_direction = sun_direction.normalize();
	const mat4 inverse_camera_matrix = camera.matrix().inverse();
	
	// Calculate 8 corners in world-space
	float4 frustum_corners[8] = {
		inverse_camera_matrix * float4(-1, -1, 0, 1),
		inverse_camera_matrix * float4( 1, -1, 0, 1),
		inverse_camera_matrix * float4(-1,  1, 0, 1),
		inverse_camera_matrix * float4( 1,  1, 0, 1),

		inverse_camera_matrix * float4(-1, -1, 1, 1),
		inverse_camera_matrix * float4( 1, -1, 1, 1),
		inverse_camera_matrix * float4(-1,  1, 1, 1),
		inverse_camera_matrix * float4( 1,  1, 1, 1),
	};

	for (auto& corner : frustum_corners) {
		corner /= corner.w;
	}

	// Convert corners to temp light-view-space
	const mat4 temp_ligth_view_matrix = mat4::look_at({}, sun_direction, { 0, 1, 0 });
	for (auto& corner : frustum_corners) {
		corner = temp_ligth_view_matrix * corner;
	}

	// Find min-max x and y in light-space
	float2 min_xy = {  INFINITY,  INFINITY };
	float2 max_xy = { -INFINITY, -INFINITY };
	float min_z = INFINITY;
	for (const auto& corner : frustum_corners) {
		min_xy.x = std::min(min_xy.x, corner.x);
		min_xy.y = std::min(min_xy.y, corner.y);

		max_xy.x = std::max(max_xy.x, corner.x);
		max_xy.y = std::max(max_xy.y, corner.y);

		min_z = std::min(min_z, corner.z);
	}

	// Find center of near plane in light-space
	float3 light_position = {
		(min_xy.x + max_xy.x) * 0.5f,
		(min_xy.y + max_xy.y) * 0.5f,
		min_z
	};

	// Convert temp light-space to world-space
	const mat4 temp_ligth_view_matrix_inverse = temp_ligth_view_matrix.inverse();
	light_position = (temp_ligth_view_matrix_inverse * float4(light_position, 1)).xyz;
	for (auto& corner : frustum_corners) {
		corner = temp_ligth_view_matrix_inverse * corner;
	}

	// Convert corners to proper light-view-space
	const mat4 light_view_matrix = mat4::look_at(light_position, light_position + sun_direction, { 0, 1, 0 });
	for (auto& corner : frustum_corners) {
		corner = light_view_matrix * corner;
	}

	// Find proper coordinates of frustum in light-space
	float3 min_xyz = float3::splash(+INFINITY);
	float3 max_xyz = float3::splash(-INFINITY);
	for (const auto& corner : frustum_corners) {
		min_xyz.x = std::min(min_xyz.x, corner.x);
		min_xyz.y = std::min(min_xyz.y, corner.y);
		min_xyz.z = std::min(min_xyz.z, corner.z);

		max_xyz.x = std::max(max_xyz.x, corner.x);
		max_xyz.y = std::max(max_xyz.y, corner.y);
		max_xyz.z = std::max(max_xyz.z, corner.z);
	}

	const mat4 light_projection_matrix = mat4::orthographic(
		min_xyz.x, max_xyz.x,
		min_xyz.y, max_xyz.y,
		min_xyz.z, max_xyz.z
	);

	return light_projection_matrix * light_view_matrix;
}

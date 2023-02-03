#include "render/light/directional_light.h"


kl::directional_light::directional_light(gpu* gpu, const int map_resolution)
	: gpu_(gpu), map_resolution_(map_resolution)
{
	dx::texture_descriptor shadow_map_descriptor = {};
	shadow_map_descriptor.Width = map_resolution;
	shadow_map_descriptor.Height = map_resolution;
	shadow_map_descriptor.MipLevels = 1;
	shadow_map_descriptor.ArraySize = 1;
	shadow_map_descriptor.Format = DXGI_FORMAT_R32_TYPELESS;
	shadow_map_descriptor.SampleDesc.Count = 1;
	shadow_map_descriptor.Usage = D3D11_USAGE_DEFAULT;
	shadow_map_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	dx::depth_view_descriptor shadow_depth_view_descriptor = {};
	shadow_depth_view_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
	shadow_depth_view_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	dx::shader_view_descriptor shadow_shader_view_descriptor = {};
	shadow_shader_view_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
	shadow_shader_view_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shadow_shader_view_descriptor.Texture2D.MipLevels = 1;

	for (int i = 0; i < MAP_COUNT; i++) {
		dx::texture shadow_texture = gpu->new_texture(&shadow_map_descriptor);
		shadow_depth_views_[i] = gpu->new_depth_view(shadow_texture, &shadow_depth_view_descriptor);
		shadow_shader_views_[i] = gpu->new_shader_view(shadow_texture, &shadow_shader_view_descriptor);
		gpu->destroy(shadow_texture);
	}
}

kl::directional_light::~directional_light()
{
	for (int i = 0; i < MAP_COUNT; i++) {
		gpu_->destroy( shadow_depth_views_[i]);
		gpu_->destroy(shadow_shader_views_[i]);
	}
}

void kl::directional_light::set_direction(const float3& direction)
{
	direction_ = direction.normalize();
}

kl::float3 kl::directional_light::get_direction() const
{
	return direction_;
}

int kl::directional_light::get_map_resolution() const
{
	return map_resolution_;
}

const kl::dx::depth_view* kl::directional_light::get_depth_views() const
{
	return shadow_depth_views_;
}

const kl::dx::shader_view* kl::directional_light::get_shader_views() const
{
	return shadow_shader_views_;
}

kl::dx::depth_view kl::directional_light::get_depth_view(const int cascade_index) const
{
	return shadow_depth_views_[cascade_index];
}

kl::dx::shader_view kl::directional_light::get_shader_view(const int cascade_index) const
{
	return shadow_shader_views_[cascade_index];
}

kl::mat4 kl::directional_light::get_matrix(camera camera, const int cascade_index) const
{
	const float plane_diff = camera.far_plane - camera.near_plane;
	camera.far_plane  = camera.near_plane + plane_diff * CASCADE_SPLITS[cascade_index + 1];
	camera.near_plane = camera.near_plane + plane_diff * CASCADE_SPLITS[cascade_index + 0];
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
	const mat4 temp_ligth_view_matrix = mat4::look_at({}, direction_, { 0, 1, 0 });
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
	const mat4 light_view_matrix = mat4::look_at(light_position, light_position + direction_, { 0, 1, 0 });
	for (auto& corner : frustum_corners) {
		corner = light_view_matrix * corner;
	}

	// Find proper coordinates of frustum in light-space
	float3 max_xyz = float3::splash(-INFINITY);
	for (const auto& corner : frustum_corners) {
		max_xyz.x = std::max(max_xyz.x, corner.x);
		max_xyz.y = std::max(max_xyz.y, corner.y);
		max_xyz.z = std::max(max_xyz.z, corner.z);
	}

	// Calculate final orthographic projection
	const mat4 light_projection_matrix = mat4::orthographic(
		-max_xyz.x, max_xyz.x,
		-max_xyz.x, max_xyz.x,
		-max_xyz.z, max_xyz.z
	);

	return light_projection_matrix * light_view_matrix;
}
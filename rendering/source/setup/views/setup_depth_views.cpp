#include "setup/setup_state.h"


void setup_depth_views(state_machine* state)
{
	kl::dx::texture_descriptor shadow_map_descriptor = {};
	shadow_map_descriptor.Width = shadow_map_resolution;
	shadow_map_descriptor.Height = shadow_map_resolution;
	shadow_map_descriptor.MipLevels = 1;
	shadow_map_descriptor.ArraySize = 1;
	shadow_map_descriptor.Format = DXGI_FORMAT_R32_TYPELESS;
	shadow_map_descriptor.SampleDesc.Count = 1;
	shadow_map_descriptor.Usage = D3D11_USAGE_DEFAULT;
	shadow_map_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	auto shadow_map_texture = state->gpu->new_texture(&shadow_map_descriptor);

	kl::dx::depth_view_descriptor shadow_depth_view_descriptor = {};
	shadow_depth_view_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
	shadow_depth_view_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	state->depth_views["shadow_map"] = state->gpu->new_depth_view(shadow_map_texture, &shadow_depth_view_descriptor);

	kl::dx::shader_view_descriptor shadow_shader_view_descriptor = {};
	shadow_shader_view_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
	shadow_shader_view_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shadow_shader_view_descriptor.Texture2D.MipLevels = 1;
	state->textures["shadow_map"] = state->gpu->new_shader_view(shadow_map_texture, &shadow_shader_view_descriptor);

	state->gpu->destroy(shadow_map_texture);
}

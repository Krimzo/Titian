#include "main.h"


titian::RenderLayer::RenderLayer()
	: Layer("RenderLayer")
{}

void titian::RenderLayer::init()
{
	kl::GPU* gpu = &app_layer->gpu;

	// Meshes
	screen_mesh = gpu->create_screen_mesh();

	// Textures
	states = new RenderStates(gpu);
	render_texture = new Texture(gpu);
	picking_texture = new Texture(gpu);
	staging_texture = new Texture(gpu);
	depth_texture = new Texture(gpu);

	resize({ 1, 1 });
	resize_staging({ 1, 1 });
}

bool titian::RenderLayer::update()
{
	const TimeBomb _ = this->time_it();

	kl::GPU* gpu = &app_layer->gpu;
	Scene* scene = &game_layer->scene;

	// Clear targets
	Camera* camera = scene->get_casted<Camera>(scene->main_camera_name);
	gpu->clear_internal(background);
	gpu->clear_target_view(render_texture->target_view, camera ? kl::Float4(camera->background) : background);
	gpu->clear_target_view(picking_texture->target_view, {});
	gpu->clear_depth_view(depth_texture->depth_view, 1.0f, 0xFF);
	if (!camera)
		return true;

	// Process passes
	const kl::Int2 render_size = get_render_texture_size();
	for (auto& pass : passes) {
		gpu->set_viewport_size(render_size);
		camera->update_aspect_ratio(render_size);
		pass->process();
	}

	// Cleanup
	const kl::Int2 window_size = app_layer->window.size();
	gpu->bind_internal_views();
	gpu->set_viewport_size(window_size);
	return true;
}

void titian::RenderLayer::present() const
{
	const kl::GPU* gpu = &app_layer->gpu;
	gpu->swap_buffers(v_sync);
}

void titian::RenderLayer::resize(const kl::Int2& new_size)
{
	if (new_size.x <= 0 || new_size.y <= 0) {
		return;
	}
	if (new_size == get_render_texture_size()) {
		return;
	}
	kl::GPU* gpu = &app_layer->gpu;

	// Create render texture
	kl::dx::TextureDescriptor render_descriptor = {};
	render_descriptor.Width = new_size.x;
	render_descriptor.Height = new_size.y;
	render_descriptor.MipLevels = 1;
	render_descriptor.ArraySize = 1;
	render_descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	render_descriptor.SampleDesc.Count = 1;
	render_descriptor.Usage = D3D11_USAGE_DEFAULT;
	render_descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	render_texture->graphics_buffer = gpu->create_texture(&render_descriptor, nullptr);
	render_texture->create_target_view(nullptr);
	render_texture->create_shader_view(nullptr);

	// Create picking texture
	kl::dx::TextureDescriptor picking_descriptor = render_descriptor;
	picking_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
	picking_texture->graphics_buffer = gpu->create_texture(&picking_descriptor, nullptr);
	picking_texture->create_target_view(nullptr);
	picking_texture->create_shader_view(nullptr);

	// Create depth texture
	kl::dx::TextureDescriptor depth_descriptor = render_descriptor;
	depth_descriptor.Format = DXGI_FORMAT_R32_TYPELESS;
	depth_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depth_texture->graphics_buffer = gpu->create_texture(&depth_descriptor, nullptr);

	kl::dx::DepthViewDescriptor depth_depth_view_descriptor = {};
	depth_depth_view_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
	depth_depth_view_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_texture->create_depth_view(&depth_depth_view_descriptor);

	kl::dx::ShaderViewDescriptor depth_shader_view_descriptor = {};
	depth_shader_view_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
	depth_shader_view_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depth_shader_view_descriptor.Texture2D.MipLevels = 1;
	depth_texture->create_shader_view(&depth_shader_view_descriptor);
}

void titian::RenderLayer::resize_staging(const kl::Int2& new_size)
{
	if (new_size.x <= 0 || new_size.y <= 0) {
		return;
	}
	const kl::Int2 current_size = staging_texture->graphics_buffer_size();
	if (current_size.x == new_size.x && current_size.y == new_size.y) {
		return;
	}
	kl::GPU* gpu = &app_layer->gpu;
	staging_texture->graphics_buffer = gpu->create_staging_texture(picking_texture->graphics_buffer, new_size);
}

kl::Int2 titian::RenderLayer::get_render_texture_size() const
{
	if (render_texture) {
		return render_texture->graphics_buffer_size();
	}
	return {};
}

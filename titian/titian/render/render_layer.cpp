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
	screen_texture = new Texture(gpu);
	game_color_texture = new Texture(gpu);
	game_depth_texture = new Texture(gpu);
	editor_picking_texture = new Texture(gpu);
	editor_staging_texture = new Texture(gpu);

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
	gpu->clear_target_view(screen_texture->target_view, background);
	gpu->clear_target_view(game_color_texture->target_view, camera ? kl::Float4(camera->background) : background);
	gpu->clear_depth_view(game_depth_texture->depth_view, 1.0f, 0xFF);
	gpu->clear_target_view(editor_picking_texture->target_view, {});
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
	app_layer->gpu.swap_buffers(v_sync);
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

	// Screen texture
	kl::dx::TextureDescriptor screen_tex_descriptor{};
	screen_tex_descriptor.Width = new_size.x;
	screen_tex_descriptor.Height = new_size.y;
	screen_tex_descriptor.MipLevels = 1;
	screen_tex_descriptor.ArraySize = 1;
	screen_tex_descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	screen_tex_descriptor.SampleDesc.Count = 1;
	screen_tex_descriptor.Usage = D3D11_USAGE_DEFAULT;
	screen_tex_descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	screen_texture->graphics_buffer = gpu->create_texture(&screen_tex_descriptor, nullptr);
	screen_texture->create_target_view(nullptr);
	screen_texture->create_shader_view(nullptr);

	// Game color texture
	kl::dx::TextureDescriptor game_col_descriptor = screen_tex_descriptor;
	game_color_texture->graphics_buffer = gpu->create_texture(&game_col_descriptor, nullptr);
	game_color_texture->create_target_view(nullptr);
	game_color_texture->create_shader_view(nullptr);

	// Game depth texture
	kl::dx::TextureDescriptor  game_depth_descriptor = game_col_descriptor;
	game_depth_descriptor.Format = DXGI_FORMAT_R32_TYPELESS;
	game_depth_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	game_depth_texture->graphics_buffer = gpu->create_texture(&game_depth_descriptor, nullptr);

	kl::dx::DepthViewDescriptor game_depth_dv_descriptor{};
	game_depth_dv_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
	game_depth_dv_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	game_depth_texture->create_depth_view(&game_depth_dv_descriptor);

	kl::dx::ShaderViewDescriptor game_depth_sv_descriptor{};
	game_depth_sv_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
	game_depth_sv_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	game_depth_sv_descriptor.Texture2D.MipLevels = 1;
	game_depth_texture->create_shader_view(&game_depth_sv_descriptor);

	// Editor picking texture
	kl::dx::TextureDescriptor editor_picking_descriptor = screen_tex_descriptor;
	editor_picking_descriptor.Format = DXGI_FORMAT_R32_FLOAT;
	editor_picking_texture->graphics_buffer = gpu->create_texture(&editor_picking_descriptor, nullptr);
	editor_picking_texture->create_target_view(nullptr);
	editor_picking_texture->create_shader_view(nullptr);
}

void titian::RenderLayer::resize_staging(const kl::Int2& new_size)
{
	if (new_size.x <= 0 || new_size.y <= 0) {
		return;
	}
	const kl::Int2 current_size = editor_staging_texture->graphics_buffer_size();
	if (current_size.x == new_size.x && current_size.y == new_size.y) {
		return;
	}
	kl::GPU* gpu = &app_layer->gpu;
	editor_staging_texture->graphics_buffer = gpu->create_staging_texture(editor_picking_texture->graphics_buffer, new_size);
}

kl::Int2 titian::RenderLayer::get_render_texture_size() const
{
	if (screen_texture) {
		return screen_texture->graphics_buffer_size();
	}
	return {};
}

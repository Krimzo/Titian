#include "editor.h"


render_state::render_state(kl::object<kl::gpu> gpu, const kl::int2& size)
    : target_size(size)
{
    // Misc data
    screen_mesh = gpu->create_screen_mesh();

    // Scene render
    kl::dx::texture_descriptor render_texture_descriptor = {};
    render_texture_descriptor.Width = size.x;
    render_texture_descriptor.Height = size.y;
    render_texture_descriptor.MipLevels = 1;
    render_texture_descriptor.ArraySize = 1;
    render_texture_descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    render_texture_descriptor.SampleDesc.Count = 1;
    render_texture_descriptor.Usage = D3D11_USAGE_DEFAULT;
    render_texture_descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    kl::dx::texture render_texture = gpu->create_texture(&render_texture_descriptor, nullptr);
    render_target_view = gpu->create_target_view(render_texture, nullptr);
    render_shader_view = gpu->create_shader_view(render_texture, nullptr);

    // Scene picking
    kl::dx::texture_descriptor picking_texture_descriptor = render_texture_descriptor;
    picking_texture_descriptor.Format = DXGI_FORMAT_R32_FLOAT;

    picking_texture = gpu->create_texture(&picking_texture_descriptor, nullptr);
    picking_target_view = gpu->create_target_view(picking_texture, nullptr);
    picking_shader_view = gpu->create_shader_view(picking_texture, nullptr);
    picking_staging_texture = gpu->create_staging_texture(picking_texture, { 1, 1 });
}

void render_state::clear_targets(kl::object<kl::gpu> gpu, const kl::float4& color)
{
    gpu->clear_target_view(render_target_view, color);
    gpu->clear_target_view(picking_target_view, {});
}

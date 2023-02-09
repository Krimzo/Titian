#include "state/render_state.h"


render_state::render_state(kl::ref<kl::gpu> gpu, const kl::int2& size)
    : gpu_(gpu), target_size(size)
{
    // Misc data
    screen_mesh = gpu->generate_screen_mesh();

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

    kl::dx::texture render_texture = gpu->new_texture(&render_texture_descriptor);
    render_target_view = gpu->new_target_view(render_texture);
    render_shader_view = gpu->new_shader_view(render_texture);
    gpu->destroy(render_texture);

    // Scene picking
    kl::dx::texture_descriptor picking_texture_descriptor = render_texture_descriptor;
    picking_texture_descriptor.Format = DXGI_FORMAT_R32_FLOAT;

    picking_texture = gpu->new_texture(&picking_texture_descriptor);
    picking_target_view = gpu->new_target_view(picking_texture);
    picking_shader_view = gpu->new_shader_view(picking_texture);
    picking_staging_texture = gpu->new_staging_texture(picking_texture, { 1, 1 });
}

render_state::~render_state()
{
    // Misc data
    if (screen_mesh) {
        gpu_->destroy(screen_mesh);
        screen_mesh = nullptr;
    }

    // Scene render
    if (render_target_view) {
        gpu_->destroy(render_target_view);
        render_target_view = nullptr;
    }
    if (render_shader_view) {
        gpu_->destroy(render_shader_view);
        render_shader_view = nullptr;
    }

    // Scene picking
    if (picking_texture) {
        gpu_->destroy(picking_texture);
        picking_texture = nullptr;
    }
    if (picking_target_view) {
        gpu_->destroy(picking_target_view);
        picking_target_view = nullptr;
    }
    if (picking_staging_texture) {
        gpu_->destroy(picking_staging_texture);
        picking_staging_texture = nullptr;
    }

    gpu_ = nullptr;
}

void render_state::clear_targets(const kl::float4& color)
{
    gpu_->clear_target_view(render_target_view, color);
    gpu_->clear_target_view(picking_target_view, {});
}

export module render_state;

export import basic_state;

export class RenderState : public BasicState
{
public:
    const kl::Int2 target_size;

    // Misc data
    kl::dx::Buffer screen_mesh = nullptr;

    // Scene render
    kl::dx::TargetView render_target_view = nullptr;
    kl::dx::ShaderView render_shader_view = nullptr;

    // Scene picking
    kl::dx::Texture picking_texture = nullptr;
    kl::dx::Texture picking_staging_texture = nullptr;
    kl::dx::TargetView picking_target_view = nullptr;
    kl::dx::ShaderView picking_shader_view = nullptr;

    RenderState(const kl::Object<kl::GPU>& gpu, const kl::Int2& size)
        : target_size(size)
    {
        // Misc data
        screen_mesh = gpu->create_screen_mesh();

        // Scene render
        kl::dx::TextureDescriptor render_texture_descriptor = {};
        render_texture_descriptor.Width = size.x;
        render_texture_descriptor.Height = size.y;
        render_texture_descriptor.MipLevels = 1;
        render_texture_descriptor.ArraySize = 1;
        render_texture_descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        render_texture_descriptor.SampleDesc.Count = 1;
        render_texture_descriptor.Usage = D3D11_USAGE_DEFAULT;
        render_texture_descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        const kl::dx::Texture render_texture = gpu->create_texture(&render_texture_descriptor, nullptr);
        render_target_view = gpu->create_target_view(render_texture, nullptr);
        render_shader_view = gpu->create_shader_view(render_texture, nullptr);

        // Scene picking
        kl::dx::TextureDescriptor picking_texture_descriptor = render_texture_descriptor;
        picking_texture_descriptor.Format = DXGI_FORMAT_R32_FLOAT;

        picking_texture = gpu->create_texture(&picking_texture_descriptor, nullptr);
        picking_target_view = gpu->create_target_view(picking_texture, nullptr);
        picking_shader_view = gpu->create_shader_view(picking_texture, nullptr);
        picking_staging_texture = gpu->create_staging_texture(picking_texture, { 1, 1 });
    }

    RenderState(RenderState&) = delete;
    RenderState(RenderState&&) = delete;

    void operator=(RenderState&) = delete;
    void operator=(RenderState&&) = delete;

    void clear_targets(kl::Object<kl::GPU> gpu, const kl::Float4& color) const
    {
        gpu->clear_target_view(render_target_view, color);
        gpu->clear_target_view(picking_target_view, {});
    }
};

#include "klibrary.h"


kl::GPU::GPU(const HWND window, const bool debug, const bool video_support)
{
    UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    if (debug) {
        creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
    }
    if (video_support) {
        creation_flags |= D3D11_CREATE_DEVICE_VIDEO_SUPPORT;
    }
    constexpr D3D_FEATURE_LEVEL feature_levels[2] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
    ComRef<IDXGISwapChain> temp_chain;
    ComRef<ID3D11Device> temp_device;
    ComRef<ID3D11DeviceContext> temp_context;
    if (window) {
        RECT client_area{};
        GetClientRect(window, &client_area);
        DXGI_SWAP_CHAIN_DESC chain_descriptor{};
        chain_descriptor.BufferCount = GPU_BUFFER_COUNT;
        chain_descriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        chain_descriptor.BufferDesc.Width = client_area.right - client_area.left;
        chain_descriptor.BufferDesc.Height = client_area.bottom - client_area.top;
        chain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        chain_descriptor.OutputWindow = window;
        chain_descriptor.SampleDesc.Count = 1;
        chain_descriptor.Windowed = true;
        chain_descriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        chain_descriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creation_flags,
            feature_levels,
            (UINT) std::size(feature_levels),
            D3D11_SDK_VERSION,
            &chain_descriptor,
            &temp_chain,
            &temp_device,
            nullptr,
            &temp_context
        ) >> verify_result;
        temp_chain.as(m_chain) >> verify_result;
        temp_device.as(m_device) >> verify_result;
        temp_context.as(m_context) >> verify_result;
        assert(m_device, "Failed to create device");
        assert(m_context, "Failed to create device context");
        assert(m_chain, "Failed to create swapchain");
        bind_raster_state(create_raster_state(false, false));
        set_viewport_min_max({ 0.0f, 1.0f });
        resize_to_window(window);
    }
    else {
        D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creation_flags,
            feature_levels,
            (UINT) std::size(feature_levels),
            D3D11_SDK_VERSION,
            &temp_device,
            nullptr,
            &temp_context
        ) >> verify_result;
        temp_device.as(m_device) >> verify_result;
        temp_context.as(m_context) >> verify_result;
        assert(m_device, "Failed to create device");
        assert(m_context, "Failed to create device context");
    }
}

kl::dx::Device kl::GPU::device() const
{
    return m_device;
}

kl::dx::Context kl::GPU::context() const
{
    return m_context;
}

kl::dx::Chain kl::GPU::chain() const
{
    return m_chain;
}

UINT kl::GPU::back_index() const
{
    return m_chain->GetCurrentBackBufferIndex();
}

kl::dx::Texture kl::GPU::target_buffer(const UINT index) const
{
    dx::Texture buffer;
    m_chain->GetBuffer(index, IID_PPV_ARGS(&buffer)) >> verify_result;
    return buffer;
}

kl::dx::Texture kl::GPU::depth_buffer(const UINT index) const
{
    return m_depth_textures[index];
}

kl::dx::TargetView kl::GPU::target_view(UINT index) const
{
    return m_target_views[index];
}

kl::dx::DepthView kl::GPU::depth_view(UINT index) const
{
	return m_depth_views[index];
}

kl::dx::Texture kl::GPU::back_target_buffer() const
{
	return target_buffer(back_index());
}

kl::dx::Texture kl::GPU::back_depth_buffer() const
{
	return depth_buffer(back_index());
}

kl::dx::TargetView kl::GPU::back_target_view() const
{
	return target_view(back_index());
}

kl::dx::DepthView kl::GPU::back_depth_view() const
{
	return depth_view(back_index());
}

void kl::GPU::swap_buffers(const bool v_sync) const
{
    const UINT interval = v_sync ? 1 : 0;
    const UINT flags = (v_sync || in_fullscreen()) ? NULL : DXGI_PRESENT_ALLOW_TEARING;
    m_chain->Present(interval, flags) >> verify_result;
    bind_internal_views();
}

bool kl::GPU::in_fullscreen() const
{
    BOOL result = false;
    m_chain->GetFullscreenState(&result, nullptr);
    return bool(result);
}

void kl::GPU::set_fullscreen(const bool enabled) const
{
    m_chain->SetFullscreenState(enabled, nullptr) >> verify_result;
}

void kl::GPU::clear_internal_color(const Float4& color) const
{
    m_context->ClearRenderTargetView(back_target_view().get(), &color.x);
}

void kl::GPU::clear_internal_depth(const float depth, const UINT8 stencil) const
{
    static constexpr UINT clear_type = (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);
    m_context->ClearDepthStencilView(back_depth_view().get(), clear_type, depth, stencil);
}

void kl::GPU::clear_internal(const Float4& color) const
{
    clear_internal_color(color);
    clear_internal_depth();
}

void kl::GPU::resize_internal(const Int2 size, const DXGI_FORMAT depth_format)
{
    unbind_target_depth_views();
    for (auto& view : m_target_views) {
        view = {};
    }
    for (auto& view : m_depth_views) {
        view = {};
    }
    for (auto& target : m_d2d1_targets) {
        target = {};
    }
    m_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING) >> verify_result;

    for (int i = 0; i < GPU_BUFFER_COUNT; i++) {
        const dx::Texture texture = back_target_buffer();
        m_target_views[i] = create_target_view(texture, nullptr);

        ComRef<IDXGISurface> surface{};
        texture->QueryInterface(IID_PPV_ARGS(&surface)) >> verify_result;
        
        const D2D1_RENDER_TARGET_PROPERTIES target_properties = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
        );
        m_d2d1_factory->CreateDxgiSurfaceRenderTarget(surface.get(), target_properties, &m_d2d1_targets[i]) >> verify_result;

        m_chain->Present(0, NULL) >> verify_result;
    }

    dx::TextureDescriptor descriptor{};
    descriptor.Width = (UINT) size.x;
    descriptor.Height = (UINT) size.y;
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 1;
    descriptor.Format = depth_format;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    for (int i = 0; i < GPU_BUFFER_COUNT; i++) {
        m_depth_textures[i] = create_texture(&descriptor, nullptr);
        m_depth_views[i] = create_depth_view(m_depth_textures[i], nullptr);
    }

    bind_internal_views();
}

void kl::GPU::resize_to_window(const HWND window)
{
    RECT window_client_area{};
    GetClientRect(window, &window_client_area);
    resize_internal({ window_client_area.right, window_client_area.bottom });
    set_viewport_size({ window_client_area.right, window_client_area.bottom });
}

void kl::GPU::bind_internal_views() const
{
    bind_target_depth_view(back_target_view(), back_depth_view());
}

kl::VertexShader kl::GPU::create_vertex_shader(const std::string_view& shader_source) const
{
    const CompiledShader compiled_shader = compile_vertex_shader(shader_source);
    VertexShader holder{ this };
    holder.shader = DeviceHolder::create_vertex_shader(compiled_shader);
    return holder;
}

kl::PixelShader kl::GPU::create_pixel_shader(const std::string_view& shader_source) const
{
    const CompiledShader compiled_shader = compile_pixel_shader(shader_source);
    PixelShader holder{ this };
    holder.shader = DeviceHolder::create_pixel_shader(compiled_shader);
    return holder;
}

kl::GeometryShader kl::GPU::create_geometry_shader(const std::string_view& shader_source) const
{
    const CompiledShader compiled_shader = compile_geometry_shader(shader_source);
    GeometryShader holder{ this };
    holder.shader = DeviceHolder::create_geometry_shader(compiled_shader);
    return holder;
}

kl::ComputeShader kl::GPU::create_compute_shader(const std::string_view& shader_source) const
{
    const CompiledShader compiled_shader = compile_compute_shader(shader_source);
    ComputeShader holder{ this };
    holder.shader = DeviceHolder::create_compute_shader(compiled_shader);
    return holder;
}

kl::Shaders kl::GPU::create_shaders(const std::string_view& shader_sources, const std::vector<dx::LayoutDescriptor>& descriptors) const
{
    const CompiledShader compiled_vertex_shader = compile_vertex_shader(shader_sources);
    const CompiledShader compiled_pixel_shader = compile_pixel_shader(shader_sources);
    Shaders shaders{ this };
    shaders.input_layout = create_input_layout(compiled_vertex_shader, descriptors);
    shaders.vertex_shader = DeviceHolder::create_vertex_shader(compiled_vertex_shader);
    shaders.pixel_shader = DeviceHolder::create_pixel_shader(compiled_pixel_shader);
    return shaders;
}

void kl::GPU::draw_text() const
{
    TextRaster::draw_text(back_index());
}

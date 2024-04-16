#include "klibrary.h"


// Creation
kl::GPU::GPU(const bool debug, const bool single_threaded)
    : creation_type(GPUCreationType::COMPUTE)
{
    UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    if (debug) {
        creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
    }
    if (single_threaded) {
        creation_flags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
    }

    const D3D_FEATURE_LEVEL feature_levels[1] = { D3D_FEATURE_LEVEL_11_1 };
    D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creation_flags,
        feature_levels,
        (UINT) std::size(feature_levels),
        D3D11_SDK_VERSION,
        &m_device,
        nullptr,
        &m_context
    ) >> verify_result;
    assert(m_device, "Failed to create device");
    assert(m_context, "Failed to create device context");
}

kl::GPU::GPU(const HWND window, const bool debug, const bool single_threaded)
    : creation_type(GPUCreationType::RENDER)
{
    RECT window_client_area{};
    GetClientRect(window, &window_client_area);

    DXGI_SWAP_CHAIN_DESC chain_descriptor{};
    chain_descriptor.BufferCount = BUFFER_COUNT;
    chain_descriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    chain_descriptor.BufferDesc.Width = window_client_area.right;
    chain_descriptor.BufferDesc.Height = window_client_area.bottom;
    chain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    chain_descriptor.OutputWindow = window;
    chain_descriptor.SampleDesc.Count = 1;
    chain_descriptor.Windowed = true;
    chain_descriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    chain_descriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    if (debug) {
        creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
    }
    if (single_threaded) {
        creation_flags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
    }

    const D3D_FEATURE_LEVEL feature_levels[1] = { D3D_FEATURE_LEVEL_11_1 };
    ComPtr<IDXGISwapChain> temp_chain{};
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
        &m_device,
        nullptr,
        &m_context
    ) >> verify_result;
    temp_chain.As(&m_chain) >> verify_result;
    assert(m_device, "Failed to create device");
    assert(m_context, "Failed to create device context");
    assert(m_chain, "Failed to create swapchain");

    bind_raster_state(create_raster_state(false, false));
    set_viewport_min_max({ 0.0f, 1.0f });
    resize_to_window(window);
}

kl::GPU::~GPU()
{
    if (m_chain) {
        m_chain->SetFullscreenState(false, nullptr);
    }
}

// Get
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

kl::dx::TargetView kl::GPU::internal_target() const
{
    const UINT index = m_chain->GetCurrentBackBufferIndex();
    return m_target_views[index];
}

kl::dx::DepthView kl::GPU::internal_depth() const
{
    const UINT index = m_chain->GetCurrentBackBufferIndex();
    return m_depth_views[index];
}

// Chain
kl::dx::Texture kl::GPU::back_buffer() const
{
    dx::Texture buffer = nullptr;
    m_chain->GetBuffer(0, IID_PPV_ARGS(&buffer)) >> verify_result;
    return buffer;
}

void kl::GPU::swap_buffers(const bool v_sync) const
{
    m_chain->Present(v_sync, NULL) >> verify_result;
    bind_internal_views();
}

bool kl::GPU::in_fullscreen() const
{
    BOOL result = false;
    IDXGIOutput* ignored = nullptr;
    m_chain->GetFullscreenState(&result, &ignored);
    return static_cast<bool>(result);
}

void kl::GPU::set_fullscreen(const bool enabled) const
{
    m_chain->SetFullscreenState(enabled, nullptr) >> verify_result;
}

// Internal buffers
void kl::GPU::clear_internal_color(const Float4& color) const
{
    m_context->ClearRenderTargetView(internal_target().Get(), color);
}

void kl::GPU::clear_internal_depth(const float depth, const UINT8 stencil) const
{
    static constexpr UINT clear_type = (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);
    m_context->ClearDepthStencilView(internal_depth().Get(), clear_type, depth, stencil);
}

void kl::GPU::clear_internal(const Float4& color) const
{
    clear_internal_color(color);
    clear_internal_depth();
}

void kl::GPU::resize_internal(const Int2& size, const DXGI_FORMAT depth_format)
{
    // Cleanup
    unbind_target_depth_views();
    for (auto& view : m_target_views) {
        view.Reset();
    }
    for (auto& view : m_depth_views) {
        view.Reset();
    }
    for (auto& target : m_d2d1_targets) {
        target.Reset();
    }
    m_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, NULL) >> verify_result;

    // Target buffers
    for (int i = 0; i < BUFFER_COUNT; i++) {
        // Render target
        const dx::Texture texture = back_buffer();
        m_target_views[i] = create_target_view(texture, nullptr);

        // Surface
        ComPtr<IDXGISurface> surface{};
        texture->QueryInterface(IID_PPV_ARGS(&surface)) >> verify_result;
        
        // Text raster target
        const D2D1_RENDER_TARGET_PROPERTIES target_properties = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
        );
        m_d2d1_factory->CreateDxgiSurfaceRenderTarget(surface.Get(), target_properties, &m_d2d1_targets[i]) >> verify_result;

        // Swap
        m_chain->Present(0, NULL) >> verify_result;
    }

    // Depth buffers
    dx::TextureDescriptor descriptor{};
    descriptor.Width = (UINT) size.x;
    descriptor.Height = (UINT) size.y;
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 1;
    descriptor.Format = depth_format;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    for (auto& view : m_depth_views) {
        const dx::Texture texture = create_texture(&descriptor, nullptr);
        view = create_depth_view(texture, nullptr);
    }

    // Rebind
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
    bind_target_depth_views({ internal_target() }, internal_depth());
}

// Shader helper
kl::ShaderHolder<kl::dx::VertexShader> kl::GPU::create_vertex_shader(const std::string& shader_source) const
{
    const CompiledShader compiled_shader = compile_vertex_shader(shader_source);
    return { this, DeviceHolder::create_vertex_shader(compiled_shader) };
}

kl::ShaderHolder<kl::dx::GeometryShader> kl::GPU::create_geometry_shader(const std::string& shader_source) const
{
    const CompiledShader compiled_shader = compile_geometry_shader(shader_source);
    return { this, DeviceHolder::create_geometry_shader(compiled_shader) };
}

kl::ShaderHolder<kl::dx::PixelShader> kl::GPU::create_pixel_shader(const std::string& shader_source) const
{
    const CompiledShader compiled_shader = compile_pixel_shader(shader_source);
    return { this, DeviceHolder::create_pixel_shader(compiled_shader) };
}

kl::ShaderHolder<kl::dx::ComputeShader> kl::GPU::create_compute_shader(const std::string& shader_source) const
{
    const CompiledShader compiled_shader = compile_compute_shader(shader_source);
    return { this, DeviceHolder::create_compute_shader(compiled_shader) };
}

kl::RenderShaders kl::GPU::create_render_shaders(const std::string& shader_sources, const std::vector<dx::LayoutDescriptor>& descriptors) const
{
    const CompiledShader compiled_vertex_shader = compile_vertex_shader(shader_sources);
    const CompiledShader compiled_pixel_shader = compile_pixel_shader(shader_sources);

    RenderShaders shaders{};
    shaders.input_layout = create_input_layout(compiled_vertex_shader, descriptors);
    shaders.vertex_shader = { this, DeviceHolder::create_vertex_shader(compiled_vertex_shader) };
    shaders.pixel_shader = { this, DeviceHolder::create_pixel_shader(compiled_pixel_shader) };
    return shaders;
}

void kl::GPU::draw_text() const
{
    const UINT index = m_chain->GetCurrentBackBufferIndex();
    TextRaster::draw_text(index);
}

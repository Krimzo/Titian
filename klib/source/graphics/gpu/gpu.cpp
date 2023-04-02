#include "graphics/gpu/gpu.h"

#include "utility/utility.h"


// Creation
kl::gpu::gpu()
    : creation_type(gpu_creation_type::compute)
{
    D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        NULL,
        nullptr,
        NULL,
        D3D11_SDK_VERSION,
        &device_,
        nullptr,
        &context_
    );
    error_check(!device_, "Failed to create device");
    error_check(!context_, "Failed to create device context");
}

kl::gpu::gpu(const HWND window)
    : creation_type(gpu_creation_type::render)
{
    RECT window_client_area = {};
    GetClientRect(window, &window_client_area);

    DXGI_SWAP_CHAIN_DESC chain_descriptor = {};
    chain_descriptor.BufferCount = 1;
    chain_descriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    chain_descriptor.BufferDesc.Width = window_client_area.right;
    chain_descriptor.BufferDesc.Height = window_client_area.bottom;
    chain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    chain_descriptor.OutputWindow = window;
    chain_descriptor.SampleDesc.Count = 1;
    chain_descriptor.Windowed = true;
    chain_descriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        NULL,
        nullptr,
        NULL,
        D3D11_SDK_VERSION,
        &chain_descriptor,
        &chain_,
        &device_,
        nullptr,
        &context_
    );
    error_check(!device_, "Failed to create device");
    error_check(!context_, "Failed to create device context");
    error_check(!chain_, "Failed to create swapchain");

    bind_raster_state(create_raster_state(false, false));
    set_viewport_min_max({ 0.0f, 1.0f });
    resize_to_window(window);
}

kl::gpu::~gpu()
{
    if (chain_) {
        chain_->SetFullscreenState(false, nullptr);
    }
}

// Get
kl::dx::device kl::gpu::device() const
{
    return device_;
}

kl::dx::context kl::gpu::context() const
{
    return context_;
}

kl::dx::chain kl::gpu::chain() const
{
    return chain_;
}

kl::dx::target_view kl::gpu::get_internal_target() const
{
    return target_view_;
}

kl::dx::depth_view kl::gpu::get_internal_depth() const
{
    return depth_view_;
}

// Chain
kl::dx::texture kl::gpu::get_back_buffer() const
{
    dx::texture buffer = nullptr;
    const long result = chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), &buffer);
    warning_check(!buffer, format("Failed to get backbuffer texture. Result: 0x", std::hex, result));
    return buffer;
}

void kl::gpu::swap_buffers(const bool v_sync) const
{
    chain_->Present(v_sync, NULL);
}

bool kl::gpu::in_fullscreen() const
{
    BOOL result = false;
    IDXGIOutput* ignored = nullptr;
    chain_->GetFullscreenState(&result, &ignored);
    return (bool) result;
}

void kl::gpu::set_fullscreen(const bool enabled) const
{
    chain_->SetFullscreenState(enabled, nullptr);
}

// Internal buffers
void kl::gpu::clear_internal_color(const float4& color) const
{
    context_->ClearRenderTargetView(target_view_.Get(), color);
}

void kl::gpu::clear_internal_depth(const float depth, const UINT8 stencil) const
{
    static const UINT clear_type = (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);
    context_->ClearDepthStencilView(depth_view_.Get(), clear_type, depth, stencil);
}

void kl::gpu::clear_internal(const float4& color) const
{
    clear_internal_color(color);
    clear_internal_depth();
}

void kl::gpu::resize_internal(const int2& size)
{
    // Cleanup
    unbind_target_depth_views();
    target_view_ = nullptr;
    depth_view_ = nullptr;
    chain_->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, NULL);

    // Render buffer
    auto render_texture = get_back_buffer();
    target_view_ = create_target_view(render_texture, nullptr);

    // Depth buffer
    dx::texture_descriptor descriptor = {};
    descriptor.Width = size.x;
    descriptor.Height = size.y;
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 1;
    descriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    auto depth_texture = create_texture(&descriptor, nullptr);
    depth_view_ = create_depth_view(depth_texture, nullptr);

    // Rebind
    bind_internal_views();
}

void kl::gpu::resize_to_window(HWND window)
{
    RECT window_client_area = {};
    GetClientRect(window, &window_client_area);
    resize_internal({ window_client_area.right, window_client_area.bottom });
    set_viewport_size({ window_client_area.right, window_client_area.bottom });
}

void kl::gpu::bind_internal_views() const
{
    bind_target_depth_views({ target_view_ }, depth_view_);
}

// Shader helper
kl::dx::vertex_shader kl::gpu::create_vertex_shader(const std::string& shader_source)
{
    const compiled_shader compiled_shader = compile_vertex_shader(shader_source);
    return device_holder::create_vertex_shader(compiled_shader);
}

kl::dx::geometry_shader kl::gpu::create_geometry_shader(const std::string& shader_source)
{
    const compiled_shader compiled_shader = compile_geometry_shader(shader_source);
    return device_holder::create_geometry_shader(compiled_shader);
}

kl::dx::pixel_shader kl::gpu::create_pixel_shader(const std::string& shader_source)
{
    const compiled_shader compiled_shader = compile_pixel_shader(shader_source);
    return device_holder::create_pixel_shader(compiled_shader);
}

kl::dx::compute_shader kl::gpu::create_compute_shader(const std::string& shader_source)
{
    const compiled_shader compiled_shader = compile_compute_shader(shader_source);
    return device_holder::create_compute_shader(compiled_shader);
}

kl::render_shaders kl::gpu::create_render_shaders(const std::string& shader_sources)
{
    const compiled_shader compiled_vertex_shader = compile_vertex_shader(shader_sources);
    const compiled_shader compiled_pixel_shader = compile_pixel_shader(shader_sources);
    return device_holder::create_render_shaders(compiled_vertex_shader, compiled_pixel_shader);
}

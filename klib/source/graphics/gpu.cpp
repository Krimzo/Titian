#include "graphics/gpu.h"

#include "utility/console.h"


kl::gpu::gpu()
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

    for (int i = 0; i < cbuffer_predefined_size; i++) {
        compute_const_buffers_[i] = new_const_buffer((i + 1) * 16);
    }

    creation_type_ = gpu_creation_type::compute;
}

kl::gpu::gpu(const HWND window)
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

    context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    resize_internal({ window_client_area.right, window_client_area.bottom });
    set_viewport({ window_client_area.right, window_client_area.bottom });
    bind_raster_state(new_raster_state(false, false));

    for (int i = 0; i < cbuffer_predefined_size; i++) {
        const int buffer_size = (i + 1) * 16;
        vertex_const_buffers_[i] = new_const_buffer(buffer_size);
        pixel_const_buffers_[i] = new_const_buffer(buffer_size);
        compute_const_buffers_[i] = new_const_buffer(buffer_size);
    }

    creation_type_ = gpu_creation_type::render;
}

kl::gpu::~gpu()
{
    for (auto& ref : children_) {
        ref->Release();
    }
    children_.clear();

    if (creation_type_ == gpu_creation_type::render) {
        chain_->SetFullscreenState(false, nullptr);
        chain_->Release();
    }

    context_->Release();
    device_->Release();
}

kl::gpu_creation_type kl::gpu::type() const
{
    return creation_type_;
}

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

void kl::gpu::set_viewport(const int2& size) const
{
    set_viewport({}, size);
}

void kl::gpu::set_viewport(const int2& position, const int2& size) const
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = float(position.x);
    viewport.TopLeftY = float(position.y);
    viewport.Width = float(size.x);
    viewport.Height = float(size.y);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    context_->RSSetViewports(1, &viewport);
}

void kl::gpu::unbind_all_targets() const
{
    context_->OMSetRenderTargets(0, nullptr, nullptr);
}

void kl::gpu::bind_internal_targets() const
{
    context_->OMSetRenderTargets(1, &frame_buffer_, depth_buffer_);
}

void kl::gpu::bind_targets(const std::vector<dx::target_view>& targets, const dx::depth_view depth_view) const
{
    context_->OMSetRenderTargets(UINT(targets.size()), targets.data(), depth_view ? depth_view : depth_buffer_);
}

void kl::gpu::bind_targets_with_internal(const std::vector<dx::target_view>& additional_targets, const dx::depth_view depth_view) const
{
    std::vector combined_targets = {frame_buffer_};
    combined_targets.insert(combined_targets.end(), additional_targets.begin(), additional_targets.end());
    context_->OMSetRenderTargets(UINT(combined_targets.size()), combined_targets.data(), depth_view ? depth_view : depth_buffer_);
}

void kl::gpu::resize_internal(const int2& size)
{
    unbind_all_targets();

    if (frame_buffer_) {
        destroy(frame_buffer_);
    }
    if (depth_buffer_) {
        destroy(depth_buffer_);
    }

    chain_->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, NULL);

    const dx::texture backbuffer_texture = get_back_buffer();
    frame_buffer_ = new_target_view(backbuffer_texture);
    destroy(backbuffer_texture);

    dx::texture_descriptor descriptor = {};
    descriptor.Width = size.x;
    descriptor.Height = size.y;
    descriptor.MipLevels = 1;
    descriptor.ArraySize = 1;
    descriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descriptor.SampleDesc.Count = 1;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    const dx::texture depth_texture = new_texture(&descriptor);
    depth_buffer_ = new_depth_view(depth_texture);
    destroy(depth_texture);

    bind_internal_targets();
}

void kl::gpu::clear_internal_color(const float4& color) const
{
    clear_target_view(frame_buffer_, color);
}

void kl::gpu::clear_internal_depth(const float value) const
{
    clear_depth_view(depth_buffer_, value);
}

void kl::gpu::clear_internal() const
{
    clear_internal_color(float4(colors::gray));
    clear_internal_depth(1.0f);
}

void kl::gpu::swap_buffers(const bool v_sync) const
{
    chain_->Present(v_sync, NULL);
}

void kl::gpu::copy_resource(const dx::resource destination, const dx::resource source) const
{
    context_->CopyResource(destination, source);
}

void kl::gpu::read_from_resource(void* cpu_buffer, const dx::resource cpu_read_resource, const int byte_size) const
{
    dx::mapped_subresource_descriptor mapped_subresource = {};
    context_->Map(cpu_read_resource, 0, D3D11_MAP_READ, NULL, &mapped_subresource);
    memcpy(cpu_buffer, mapped_subresource.pData, byte_size);
    context_->Unmap(cpu_read_resource, NULL);
}

void kl::gpu::write_to_resource(const dx::resource cpu_write_resource, const void* data, const int byte_size, const bool discard) const
{
    dx::mapped_subresource_descriptor mapped_subresource = {};
    context_->Map(cpu_write_resource, 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, NULL, &mapped_subresource);
    memcpy(mapped_subresource.pData, data, byte_size);
    context_->Unmap(cpu_write_resource, NULL);
}

void kl::gpu::destroy(IUnknown* child)
{
    if (children_.contains(child)) {
        children_.erase(child);
        child->Release();
    }
}

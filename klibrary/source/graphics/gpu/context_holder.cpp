#include "klibrary.h"


kl::ContextHolder::ContextHolder()
{}

// Viewport
void kl::ContextHolder::set_viewport_position(const Int2& position) const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);

    viewport.TopLeftX = static_cast<float>(position.x);
    viewport.TopLeftY = static_cast<float>(position.y);
    m_context->RSSetViewports(1, &viewport);
}

kl::Int2 kl::ContextHolder::viewport_position() const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);
    return {
        static_cast<int>(viewport.TopLeftX),
        static_cast<int>(viewport.TopLeftY),
    };
}

void kl::ContextHolder::set_viewport_size(const Int2& size) const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);

    viewport.Width = static_cast<float>(size.x);
    viewport.Height = static_cast<float>(size.y);
    m_context->RSSetViewports(1, &viewport);
}

kl::Int2 kl::ContextHolder::viewport_size() const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);
    return {
        static_cast<int>(viewport.Width),
        static_cast<int>(viewport.Height),
    };
}

void kl::ContextHolder::set_viewport_min_max(const Float2& min_max) const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);

    viewport.MinDepth = min_max.x;
    viewport.MaxDepth = min_max.y;
    m_context->RSSetViewports(1, &viewport);
}

kl::Float2 kl::ContextHolder::viewport_min_max() const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);
    return { viewport.MinDepth, viewport.MaxDepth };
}

// States
void kl::ContextHolder::bind_raster_state(const dx::RasterState& state) const
{
    m_context->RSSetState(state.Get());
}

void kl::ContextHolder::unbind_raster_state() const
{
    bind_raster_state(nullptr);
}

void kl::ContextHolder::bind_depth_state(const dx::DepthState& state, const UINT stencil_ref) const
{
    m_context->OMSetDepthStencilState(state.Get(), stencil_ref);
}

void kl::ContextHolder::unbind_depth_state() const
{
    bind_depth_state(nullptr, 0x00);
}

void kl::ContextHolder::bind_sampler_state_for_pixel_shader(const dx::SamplerState& state, const UINT slot) const
{
    m_context->PSSetSamplers(slot, 1, state.GetAddressOf());
}

void kl::ContextHolder::unbind_sampler_state_for_pixel_shader(const UINT slot) const
{
    bind_sampler_state_for_pixel_shader(nullptr, slot);
}

void kl::ContextHolder::bind_sampler_state_for_compute_shader(const dx::SamplerState& state, const UINT slot) const
{
    m_context->CSSetSamplers(slot, 1, state.GetAddressOf());
}

void kl::ContextHolder::unbind_sampler_state_for_compute_shader(const UINT slot) const
{
    bind_sampler_state_for_compute_shader(nullptr, slot);
}

void kl::ContextHolder::bind_blend_state(const dx::BlendState& state, const float* factor, const UINT mask) const
{
    m_context->OMSetBlendState(state.Get(), factor, mask);
}

void kl::ContextHolder::unbind_blend_state() const
{
    bind_blend_state(nullptr);
}

// Resources
void kl::ContextHolder::copy_resource(const dx::Resource& destination, const dx::Resource& source) const
{
    m_context->CopyResource(destination.Get(), source.Get());
}

void kl::ContextHolder::read_from_resource(void* cpu_buffer, const dx::Resource& gpu_buffer, SIZE_T byte_size) const
{
    dx::MappedSubresourceDescriptor mapped_subresource{};
    m_context->Map(gpu_buffer.Get(), 0, D3D11_MAP_READ, NULL, &mapped_subresource) >> verify_result;
    if (cpu_buffer && mapped_subresource.pData) {
        memcpy(cpu_buffer, mapped_subresource.pData, byte_size);
    }
    m_context->Unmap(gpu_buffer.Get(), NULL);
}

void kl::ContextHolder::write_to_resource(const dx::Resource& gpu_buffer, const void* cpu_buffer, SIZE_T byte_size, bool discard) const
{
    dx::MappedSubresourceDescriptor mapped_subresource{};
    m_context->Map(gpu_buffer.Get(), 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, NULL, &mapped_subresource) >> verify_result;
    if (mapped_subresource.pData && cpu_buffer) {
        memcpy(mapped_subresource.pData, cpu_buffer, byte_size);
    }
    m_context->Unmap(gpu_buffer.Get(), NULL);
}

// Buffers
UINT kl::ContextHolder::buffer_size(const dx::Buffer& buffer) const
{
    if (!buffer) {
        return 0;
    }
    dx::BufferDescriptor descriptor{};
    buffer->GetDesc(&descriptor);
    return descriptor.ByteWidth;
}

// Const buffers
void kl::ContextHolder::bind_cb_for_vertex_shader(const dx::Buffer& buffer, const UINT slot) const
{
    m_context->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void kl::ContextHolder::unbind_cb_for_vertex_shader(const UINT slot) const
{
    bind_cb_for_vertex_shader(nullptr, slot);
}

void kl::ContextHolder::bind_cb_for_geometry_shader(const dx::Buffer& buffer, const UINT slot) const
{
    m_context->GSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void kl::ContextHolder::unbind_cb_for_geometry_shader(const UINT slot) const
{
    bind_cb_for_geometry_shader(nullptr, slot);
}

void kl::ContextHolder::bind_cb_for_pixel_shader(const dx::Buffer& buffer, const UINT slot) const
{
    m_context->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void kl::ContextHolder::unbind_cb_for_pixel_shader(const UINT slot) const
{
    bind_cb_for_pixel_shader(nullptr, slot);
}

void kl::ContextHolder::bind_cb_for_compute_shader(const dx::Buffer& buffer, const UINT slot) const
{
    m_context->CSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void kl::ContextHolder::unbind_cb_for_compute_shader(const UINT slot) const
{
    bind_cb_for_compute_shader(nullptr, slot);
}

// Vertex buffers
UINT kl::ContextHolder::vertex_buffer_size(const dx::Buffer& buffer, const UINT stride) const
{
    return (buffer_size(buffer) / stride);
}

void kl::ContextHolder::bind_vertex_buffer(const dx::Buffer& buffer, const UINT slot, const UINT offset, const UINT stride) const
{
    m_context->IASetVertexBuffers(slot, 1, buffer.GetAddressOf(), &stride, &offset);
}

void kl::ContextHolder::unbind_vertex_buffer(const UINT slot) const
{
    bind_vertex_buffer(nullptr, slot, 0, 0);
}

// Index buffers
UINT kl::ContextHolder::index_buffer_size(const dx::Buffer& buffer) const
{
    return (buffer_size(buffer) / sizeof(uint32_t));
}

void kl::ContextHolder::bind_index_buffer(const dx::Buffer& buffer, const UINT offset) const
{
    m_context->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, offset);
}

void kl::ContextHolder::unbind_index_buffer(UINT slot) const
{
    bind_index_buffer(nullptr, 0);
}

// Draw
void kl::ContextHolder::set_draw_type(const D3D_PRIMITIVE_TOPOLOGY draw_type) const
{
    m_context->IASetPrimitiveTopology(draw_type);
}

void kl::ContextHolder::draw(const UINT vertex_count, const UINT start_index) const
{
    m_context->Draw(vertex_count, start_index);
}

void kl::ContextHolder::draw(const dx::Buffer& vertex_buffer, const D3D_PRIMITIVE_TOPOLOGY draw_type, const UINT stride) const
{
    if (!vertex_buffer) {
        return;
    }
    set_draw_type(draw_type);
    bind_vertex_buffer(vertex_buffer, 0, 0, stride);
    draw(vertex_buffer_size(vertex_buffer, stride), 0);
}

void kl::ContextHolder::draw_indexed(const UINT index_count, const UINT start_index, const INT base_vertex) const
{
    m_context->DrawIndexed(index_count, start_index, base_vertex);
}

void kl::ContextHolder::draw_indexed(const dx::Buffer& vertex_buffer, const dx::Buffer& index_buffer, const D3D_PRIMITIVE_TOPOLOGY draw_type, const UINT stride) const
{
    set_draw_type(draw_type);
    bind_vertex_buffer(vertex_buffer, 0, 0, stride);
    bind_index_buffer(index_buffer, 0);

    const UINT index_count = index_buffer_size(index_buffer);
    draw_indexed(index_count, 0, 0);
}

// Views
void kl::ContextHolder::clear_target_view(const dx::TargetView& view, const Float4& color) const
{
    m_context->ClearRenderTargetView(view.Get(), color);
}

void kl::ContextHolder::clear_depth_view(const dx::DepthView& view, const float depth, const UINT8 stencil) const
{
    m_context->ClearDepthStencilView(view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void kl::ContextHolder::bind_target_depth_views(const std::vector<dx::TargetView>& target_views, const dx::DepthView& depth_view) const
{
    std::vector<ID3D11RenderTargetView*> list_of_targets(target_views.size());
    for (size_t i = 0; i < list_of_targets.size(); i++) {
        list_of_targets[i] = target_views[i].Get();
    }
    m_context->OMSetRenderTargets((UINT) target_views.size(), list_of_targets.data(), depth_view.Get());
}

void kl::ContextHolder::unbind_target_depth_views() const
{
    m_context->OMSetRenderTargets(0, nullptr, nullptr);
}

void kl::ContextHolder::bind_shader_view_for_pixel_shader(const dx::ShaderView& view, const UINT slot) const
{
    m_context->PSSetShaderResources(slot, 1, view.GetAddressOf());
}

void kl::ContextHolder::unbind_shader_view_for_pixel_shader(const UINT slot) const
{
    bind_shader_view_for_pixel_shader(nullptr, slot);
}

void kl::ContextHolder::bind_shader_view_for_compute_shader(const dx::ShaderView& view, const UINT slot) const
{
    m_context->CSSetShaderResources(slot, 1, view.GetAddressOf());
}

void kl::ContextHolder::unbind_shader_view_for_compute_shader(const UINT slot) const
{
    bind_shader_view_for_compute_shader(nullptr, slot);
}

void kl::ContextHolder::bind_access_view_for_compute_shader(const dx::AccessView& view, const UINT slot, const UINT* initial_counts) const
{
    m_context->CSSetUnorderedAccessViews(slot, 1, view.GetAddressOf(), initial_counts);
}

void kl::ContextHolder::unbind_access_view_for_compute_shader(const UINT slot) const
{
    bind_access_view_for_compute_shader(nullptr, slot);
}

// Shaders
void kl::ContextHolder::bind_input_layout(const dx::InputLayout& input_layout) const
{
    m_context->IASetInputLayout(input_layout.Get());
}

void kl::ContextHolder::unbind_input_layout() const
{
    bind_input_layout(nullptr);
}

void kl::ContextHolder::bind_vertex_shader(const dx::VertexShader& shader) const
{
    m_context->VSSetShader(shader.Get(), nullptr, 0);
}

void kl::ContextHolder::unbind_vertex_shader() const
{
    bind_vertex_shader(nullptr);
}

void kl::ContextHolder::bind_geometry_shader(const dx::GeometryShader& shader) const
{
    m_context->GSSetShader(shader.Get(), nullptr, 0);
}

void kl::ContextHolder::unbind_geometry_shader() const
{
    bind_geometry_shader(nullptr);
}

void kl::ContextHolder::bind_pixel_shader(const dx::PixelShader& shader) const
{
    m_context->PSSetShader(shader.Get(), nullptr, 0);
}

void kl::ContextHolder::unbind_pixel_shader() const
{
    bind_pixel_shader(nullptr);
}

void kl::ContextHolder::bind_compute_shader(const dx::ComputeShader& shader) const
{
    m_context->CSSetShader(shader.Get(), nullptr, 0);
}

void kl::ContextHolder::unbind_compute_shader() const
{
    bind_compute_shader(nullptr);
}

void kl::ContextHolder::bind_render_shaders(const RenderShaders& shaders) const
{
    bind_input_layout(shaders.input_layout);
    bind_vertex_shader(shaders.vertex_shader);
    bind_pixel_shader(shaders.pixel_shader);
}

void kl::ContextHolder::unbind_render_shaders() const
{
    bind_render_shaders({});
}

void kl::ContextHolder::dispatch_compute_shader(const UINT x, const UINT y, const UINT z) const
{
    m_context->Dispatch(x, y, z);
}

void kl::ContextHolder::execute_compute_shader(const dx::ComputeShader& shader, const UINT x, const UINT y, const UINT z) const
{
    bind_compute_shader(shader);
    dispatch_compute_shader(x, y, z);
}

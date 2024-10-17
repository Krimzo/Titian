#include "klibrary.h"


void kl::ContextHolder::set_viewport_position(const Int2 position) const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);
    viewport.TopLeftX = float(position.x);
    viewport.TopLeftY = float(position.y);
    m_context->RSSetViewports(1, &viewport);
}

kl::Int2 kl::ContextHolder::viewport_position() const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);
    return {
        int(viewport.TopLeftX),
        int(viewport.TopLeftY),
    };
}

void kl::ContextHolder::set_viewport_size(const Int2 size) const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);
    viewport.Width = float(size.x);
    viewport.Height = float(size.y);
    m_context->RSSetViewports(1, &viewport);
}

kl::Int2 kl::ContextHolder::viewport_size() const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport{};
    m_context->RSGetViewports(&number_of_vps, &viewport);
    return { int(viewport.Width), int(viewport.Height) };
}

void kl::ContextHolder::set_viewport_min_max(const Float2 min_max) const
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

void kl::ContextHolder::bind_raster_state(const dx::RasterState& state) const
{
    m_context->RSSetState(state.get());
}

void kl::ContextHolder::unbind_raster_state() const
{
    bind_raster_state({});
}

void kl::ContextHolder::bind_depth_state(const dx::DepthState& state, const UINT stencil_ref) const
{
    m_context->OMSetDepthStencilState(state.get(), stencil_ref);
}

void kl::ContextHolder::unbind_depth_state() const
{
    bind_depth_state({}, 0x00);
}

void kl::ContextHolder::bind_sampler_state_for_pixel_shader(const dx::SamplerState& state, const UINT slot) const
{
    m_context->PSSetSamplers(slot, 1, state.address());
}

void kl::ContextHolder::unbind_sampler_state_for_pixel_shader(const UINT slot) const
{
    bind_sampler_state_for_pixel_shader({}, slot);
}

void kl::ContextHolder::bind_sampler_state_for_compute_shader(const dx::SamplerState& state, const UINT slot) const
{
    m_context->CSSetSamplers(slot, 1, state.address());
}

void kl::ContextHolder::unbind_sampler_state_for_compute_shader(const UINT slot) const
{
    bind_sampler_state_for_compute_shader({}, slot);
}

void kl::ContextHolder::bind_blend_state(const dx::BlendState& state, const float* factor, const UINT mask) const
{
    m_context->OMSetBlendState(state.get(), factor, mask);
}

void kl::ContextHolder::unbind_blend_state() const
{
    bind_blend_state({});
}

void kl::ContextHolder::copy_resource(const dx::Resource& destination, const dx::Resource& source) const
{
    m_context->CopyResource(destination.get(), source.get());
}

void kl::ContextHolder::copy_resource_region(const dx::Resource& destination, const dx::Resource& source, const Int2 src_min, const Int2 src_max, const Int2 dst) const
{
    D3D11_BOX source_box{};
    source_box.left = src_min.x;
    source_box.top = src_min.y;
    source_box.right = src_max.x;
    source_box.bottom = src_max.y;
    source_box.front = 0;
    source_box.back = 1;
	m_context->CopySubresourceRegion(destination.get(), 0, dst.x, dst.y, 0, source.get(), 0, &source_box);
}

void kl::ContextHolder::map_read_resource(const dx::Resource& resource, const std::function<void(const byte*, uint32_t)>& func) const
{
    dx::MappedSubresourceDescriptor mapped_subresource{};
    m_context->Map(resource.get(), 0, D3D11_MAP_READ, NULL, &mapped_subresource) >> verify_result;
    func((const byte*) mapped_subresource.pData, mapped_subresource.RowPitch);
    m_context->Unmap(resource.get(), 0);
}

void kl::ContextHolder::map_write_resource(const dx::Resource& resource, const std::function<void(byte*, uint32_t)>& func, const bool discard) const
{
    dx::MappedSubresourceDescriptor mapped_subresource{};
    m_context->Map(resource.get(), 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, NULL, &mapped_subresource) >> verify_result;
    func((byte*) mapped_subresource.pData, mapped_subresource.RowPitch);
    m_context->Unmap(resource.get(), 0);
}

void kl::ContextHolder::read_from_buffer(void* cpu_buffer, const dx::Buffer& gpu_buffer, const SIZE_T byte_size) const
{
    dx::MappedSubresourceDescriptor mapped_subresource{};
    m_context->Map(gpu_buffer.get(), 0, D3D11_MAP_READ, NULL, &mapped_subresource) >> verify_result;
    copy<byte>(cpu_buffer, mapped_subresource.pData, byte_size);
    m_context->Unmap(gpu_buffer.get(), 0);
}

void kl::ContextHolder::write_to_buffer(const dx::Buffer& gpu_buffer, const void* cpu_buffer, const SIZE_T byte_size, const bool discard) const
{
    dx::MappedSubresourceDescriptor mapped_subresource{};
    m_context->Map(gpu_buffer.get(), 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, NULL, &mapped_subresource) >> verify_result;
    copy<byte>(mapped_subresource.pData, cpu_buffer, byte_size);
    m_context->Unmap(gpu_buffer.get(), 0);
}

void kl::ContextHolder::read_from_texture(void* cpu_buffer, const dx::Texture& gpu_buffer, const Int2 cpu_size, const UINT element_size) const
{
    dx::MappedSubresourceDescriptor mapped_subresource{};
    m_context->Map(gpu_buffer.get(), 0, D3D11_MAP_READ, NULL, &mapped_subresource) >> verify_result;
    BYTE* out_ptr = (BYTE*) cpu_buffer;
    const BYTE* in_ptr = (const BYTE*) mapped_subresource.pData;
    const Int2 min_size = min(texture_size(gpu_buffer), cpu_size);
    for (int y = 0; y < min_size.y; y++) {
        BYTE* out_addr = out_ptr + (y * cpu_size.x * element_size);
        const BYTE* in_addr = in_ptr + (y * mapped_subresource.RowPitch);
        copy<byte>(out_addr, in_addr, min_size.x * uint64_t(element_size));
    }
    m_context->Unmap(gpu_buffer.get(), 0);
}

void kl::ContextHolder::write_to_texture(const dx::Texture& gpu_buffer, const void* cpu_buffer, const Int2 cpu_size, const UINT element_size, const bool discard) const
{
    dx::MappedSubresourceDescriptor mapped_subresource{};
    m_context->Map(gpu_buffer.get(), 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, NULL, &mapped_subresource) >> verify_result;
    BYTE* out_ptr = (BYTE*) mapped_subresource.pData;
    const BYTE* in_ptr = (const BYTE*) cpu_buffer;
    const Int2 min_size = min(texture_size(gpu_buffer), cpu_size);
    for (int y = 0; y < min_size.y; y++) {
        BYTE* out_addr = out_ptr + (y * mapped_subresource.RowPitch);
        const BYTE* in_addr = in_ptr + (y * cpu_size.x * element_size);
        copy<byte>(out_addr, in_addr, min_size.x * uint64_t(element_size));
    }
    m_context->Unmap(gpu_buffer.get(), 0);
}

UINT kl::ContextHolder::buffer_size(const dx::Buffer& buffer) const
{
    if (!buffer) {
        return 0;
    }
    dx::BufferDescriptor descriptor;
    buffer->GetDesc(&descriptor);
    return descriptor.ByteWidth;
}

kl::Int2 kl::ContextHolder::texture_size(const dx::Texture& texture) const
{
	if (!texture) {
        return {};
	}
	dx::TextureDescriptor descriptor;
	texture->GetDesc(&descriptor);
	return { (int) descriptor.Width, (int) descriptor.Height };
}

void kl::ContextHolder::bind_cb_for_vertex_shader(const dx::Buffer& buffer, const UINT slot) const
{
    m_context->VSSetConstantBuffers(slot, 1, buffer.address());
}

void kl::ContextHolder::unbind_cb_for_vertex_shader(const UINT slot) const
{
    bind_cb_for_vertex_shader({}, slot);
}

void kl::ContextHolder::bind_cb_for_geometry_shader(const dx::Buffer& buffer, const UINT slot) const
{
    m_context->GSSetConstantBuffers(slot, 1, buffer.address());
}

void kl::ContextHolder::unbind_cb_for_geometry_shader(const UINT slot) const
{
    bind_cb_for_geometry_shader({}, slot);
}

void kl::ContextHolder::bind_cb_for_pixel_shader(const dx::Buffer& buffer, const UINT slot) const
{
    m_context->PSSetConstantBuffers(slot, 1, buffer.address());
}

void kl::ContextHolder::unbind_cb_for_pixel_shader(const UINT slot) const
{
    bind_cb_for_pixel_shader({}, slot);
}

void kl::ContextHolder::bind_cb_for_compute_shader(const dx::Buffer& buffer, const UINT slot) const
{
    m_context->CSSetConstantBuffers(slot, 1, buffer.address());
}

void kl::ContextHolder::unbind_cb_for_compute_shader(const UINT slot) const
{
    bind_cb_for_compute_shader({}, slot);
}

UINT kl::ContextHolder::vertex_buffer_size(const dx::Buffer& buffer, const UINT stride) const
{
    return (buffer_size(buffer) / stride);
}

void kl::ContextHolder::bind_vertex_buffer(const dx::Buffer& buffer, const UINT slot, const UINT offset, const UINT stride) const
{
    m_context->IASetVertexBuffers(slot, 1, buffer.address(), &stride, &offset);
}

void kl::ContextHolder::unbind_vertex_buffer(const UINT slot) const
{
    bind_vertex_buffer({}, slot, 0, 0);
}

UINT kl::ContextHolder::index_buffer_size(const dx::Buffer& buffer) const
{
    return (buffer_size(buffer) / sizeof(uint32_t));
}

void kl::ContextHolder::bind_index_buffer(const dx::Buffer& buffer, const UINT offset) const
{
    m_context->IASetIndexBuffer(buffer.get(), DXGI_FORMAT_R32_UINT, offset);
}

void kl::ContextHolder::unbind_index_buffer(UINT slot) const
{
    bind_index_buffer({}, 0);
}

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
    if (!vertex_buffer)
        return;
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
    draw_indexed(index_buffer_size(index_buffer), 0, 0);
}

void kl::ContextHolder::clear_target_view(const dx::TargetView& view, const Float4& color) const
{
    m_context->ClearRenderTargetView(view.get(), &color.x);
}

void kl::ContextHolder::clear_depth_view(const dx::DepthView& view, const float depth, const UINT8 stencil) const
{
    m_context->ClearDepthStencilView(view.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void kl::ContextHolder::bind_target_depth_view(const dx::TargetView& target_view, const dx::DepthView& depth_view) const
{
    ID3D11RenderTargetView* target_views[1] = { target_view.get() };
    m_context->OMSetRenderTargets(1, target_views, depth_view.get());
}

void kl::ContextHolder::bind_target_depth_views(const std::initializer_list<ID3D11RenderTargetView*>& target_views, const dx::DepthView& depth_view) const
{
    m_context->OMSetRenderTargets((UINT) target_views.size(), target_views.begin(), depth_view.get());
}

void kl::ContextHolder::unbind_target_depth_views() const
{
    m_context->OMSetRenderTargets(0, nullptr, nullptr);
}

void kl::ContextHolder::bind_shader_view_for_vertex_shader(const dx::ShaderView& view, const UINT slot) const
{
    m_context->VSSetShaderResources(slot, 1, view.address());
}

void kl::ContextHolder::unbind_shader_view_for_vertex_shader(const UINT slot) const
{
    bind_shader_view_for_vertex_shader({}, slot);
}

void kl::ContextHolder::bind_shader_view_for_pixel_shader(const dx::ShaderView& view, const UINT slot) const
{
    m_context->PSSetShaderResources(slot, 1, view.address());
}

void kl::ContextHolder::unbind_shader_view_for_pixel_shader(const UINT slot) const
{
    bind_shader_view_for_pixel_shader({}, slot);
}

void kl::ContextHolder::bind_shader_view_for_compute_shader(const dx::ShaderView& view, const UINT slot) const
{
    m_context->CSSetShaderResources(slot, 1, view.address());
}

void kl::ContextHolder::unbind_shader_view_for_compute_shader(const UINT slot) const
{
    bind_shader_view_for_compute_shader({}, slot);
}

void kl::ContextHolder::bind_access_view_for_compute_shader(const dx::AccessView& view, const UINT slot, const UINT* initial_counts) const
{
    m_context->CSSetUnorderedAccessViews(slot, 1, view.address(), initial_counts);
}

void kl::ContextHolder::unbind_access_view_for_compute_shader(const UINT slot) const
{
    bind_access_view_for_compute_shader({}, slot);
}

void kl::ContextHolder::bind_input_layout(const dx::InputLayout& input_layout) const
{
    m_context->IASetInputLayout(input_layout.get());
}

void kl::ContextHolder::unbind_input_layout() const
{
    bind_input_layout({});
}

void kl::ContextHolder::bind_vertex_shader(const dx::VertexShader& shader) const
{
    m_context->VSSetShader(shader.get(), nullptr, 0);
}

void kl::ContextHolder::unbind_vertex_shader() const
{
    bind_vertex_shader({});
}

void kl::ContextHolder::bind_geometry_shader(const dx::GeometryShader& shader) const
{
    m_context->GSSetShader(shader.get(), nullptr, 0);
}

void kl::ContextHolder::unbind_geometry_shader() const
{
    bind_geometry_shader({});
}

void kl::ContextHolder::bind_pixel_shader(const dx::PixelShader& shader) const
{
    m_context->PSSetShader(shader.get(), nullptr, 0);
}

void kl::ContextHolder::unbind_pixel_shader() const
{
    bind_pixel_shader({});
}

void kl::ContextHolder::bind_compute_shader(const dx::ComputeShader& shader) const
{
    m_context->CSSetShader(shader.get(), nullptr, 0);
}

void kl::ContextHolder::unbind_compute_shader() const
{
    bind_compute_shader({});
}

void kl::ContextHolder::bind_shaders(const Shaders& shaders) const
{
    bind_input_layout(shaders.input_layout);
    bind_vertex_shader(shaders.vertex_shader);
    bind_pixel_shader(shaders.pixel_shader);
}

void kl::ContextHolder::unbind_shaders() const
{
    unbind_input_layout();
    unbind_vertex_shader();
    unbind_pixel_shader();
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

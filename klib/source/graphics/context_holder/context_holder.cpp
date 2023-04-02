#include "graphics/context_holder/context_holder.h"


// Creation
kl::context_holder::context_holder()
{}

kl::context_holder::~context_holder()
{}

// Viewport
void kl::context_holder::set_viewport_position(const int2& position) const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport = {};
    context_->RSGetViewports(&number_of_vps, &viewport);

    viewport.TopLeftX = (float) position.x;
    viewport.TopLeftY = (float) position.y;
    context_->RSSetViewports(1, &viewport);
}

kl::int2 kl::context_holder::get_viewport_position() const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport = {};
    context_->RSGetViewports(&number_of_vps, &viewport);
    return { (int) viewport.TopLeftX, (int) viewport.TopLeftY };
}

void kl::context_holder::set_viewport_size(const int2& size) const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport = {};
    context_->RSGetViewports(&number_of_vps, &viewport);

    viewport.Width = (float) size.x;
    viewport.Height = (float) size.y;
    context_->RSSetViewports(1, &viewport);
}

kl::int2 kl::context_holder::get_viewport_size() const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport = {};
    context_->RSGetViewports(&number_of_vps, &viewport);
    return { (int) viewport.Width, (int) viewport.Height };
}

void kl::context_holder::set_viewport_min_max(const float2& min_max) const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport = {};
    context_->RSGetViewports(&number_of_vps, &viewport);

    viewport.MinDepth = min_max.x;
    viewport.MaxDepth = min_max.y;
    context_->RSSetViewports(1, &viewport);
}

kl::float2 kl::context_holder::get_viewport_min_max() const
{
    UINT number_of_vps = 1;
    D3D11_VIEWPORT viewport = {};
    context_->RSGetViewports(&number_of_vps, &viewport);
    return { viewport.MinDepth, viewport.MaxDepth };
}

// States
void kl::context_holder::bind_raster_state(const dx::raster_state state) const
{
    context_->RSSetState(state.Get());
}

void kl::context_holder::unbind_rater_state() const
{
    bind_raster_state(nullptr);
}

void kl::context_holder::bind_depth_state(const dx::depth_state state, const UINT stencil_ref) const
{
    context_->OMSetDepthStencilState(state.Get(), stencil_ref);
}

void kl::context_holder::unbind_depth_state() const
{
    bind_depth_state(nullptr, 0x00);
}

void kl::context_holder::bind_sampler_state_for_pixel_shader(const dx::sampler_state state, const UINT slot) const
{
    context_->PSSetSamplers(slot, 1, state.GetAddressOf());
}

void kl::context_holder::unbind_sampler_state_for_pixel_shader(const UINT slot) const
{
    bind_sampler_state_for_pixel_shader(nullptr, slot);
}

void kl::context_holder::bind_sampler_state_for_compute_shader(const dx::sampler_state state, const UINT slot) const
{
    context_->CSSetSamplers(slot, 1, state.GetAddressOf());
}

void kl::context_holder::unbind_sampler_state_for_compute_shader(const UINT slot) const
{
    bind_sampler_state_for_compute_shader(nullptr, slot);
}

// Resources
void kl::context_holder::copy_resource(dx::resource destination, const dx::resource source) const
{
    context_->CopyResource(destination.Get(), source.Get());
}

void kl::context_holder::read_from_resource(void* cpu_buffer, const dx::resource gpu_buffer, SIZE_T byte_size) const
{
    dx::mapped_subresource_descriptor mapped_subresource = {};
    context_->Map(gpu_buffer.Get(), 0, D3D11_MAP_READ, NULL, &mapped_subresource);
    memcpy(cpu_buffer, mapped_subresource.pData, byte_size);
    context_->Unmap(gpu_buffer.Get(), NULL);
}

void kl::context_holder::write_to_resource(dx::resource gpu_buffer, const void* cpu_buffer, SIZE_T byte_size, bool discard) const
{
    dx::mapped_subresource_descriptor mapped_subresource = {};
    context_->Map(gpu_buffer.Get(), 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, NULL, &mapped_subresource);
    memcpy(mapped_subresource.pData, cpu_buffer, byte_size);
    context_->Unmap(gpu_buffer.Get(), NULL);
}

// Buffers
UINT kl::context_holder::get_buffer_size(const dx::buffer buffer) const
{
    if (!buffer) {
        return 0;
    }

    dx::buffer_descriptor descriptor = {};
    buffer->GetDesc(&descriptor);
    return descriptor.ByteWidth;
}

// Const buffers
void kl::context_holder::bind_cb_for_vertex_shader(const dx::buffer buffer, const UINT slot) const
{
    context_->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void kl::context_holder::unbind_cb_for_vertex_shader(const UINT slot) const
{
    bind_cb_for_vertex_shader(nullptr, slot);
}

void kl::context_holder::bind_cb_for_geometry_shader(const dx::buffer buffer, const UINT slot) const
{
    context_->GSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void kl::context_holder::unbind_cb_for_geometry_shader(const UINT slot) const
{
    bind_cb_for_geometry_shader(nullptr, slot);
}

void kl::context_holder::bind_cb_for_pixel_shader(const dx::buffer buffer, const UINT slot) const
{
    context_->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void kl::context_holder::unbind_cb_for_pixel_shader(const UINT slot) const
{
    bind_cb_for_pixel_shader(nullptr, slot);
}

void kl::context_holder::bind_cb_for_compute_shader(const dx::buffer buffer, const UINT slot) const
{
    context_->CSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
}

void kl::context_holder::unbind_cb_for_compute_shader(const UINT slot) const
{
    bind_cb_for_compute_shader(nullptr, slot);
}

// Meshes
UINT kl::context_holder::get_mesh_vertex_count(dx::buffer mesh, const UINT stride) const
{
    return (get_buffer_size(mesh) / stride);
}

void kl::context_holder::bind_mesh(const dx::buffer mesh, const UINT slot, const UINT offset, const UINT stride) const
{
    context_->IASetVertexBuffers(slot, 1, mesh.GetAddressOf(), &stride, &offset);
}

void kl::context_holder::unbind_mesh(const UINT slot) const
{
    bind_mesh(nullptr, slot, 0, 0);
}

void kl::context_holder::set_draw_type(const D3D_PRIMITIVE_TOPOLOGY draw_type) const
{
    context_->IASetPrimitiveTopology(draw_type);
}

void kl::context_holder::draw(const UINT vertex_count, const UINT start_index) const
{
    context_->Draw(vertex_count, start_index);
}

void kl::context_holder::draw_mesh(const dx::buffer mesh) const
{
    draw_mesh(mesh, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void kl::context_holder::draw_mesh(const dx::buffer mesh, const D3D_PRIMITIVE_TOPOLOGY draw_type) const
{
    draw_mesh(mesh, draw_type, sizeof(vertex));
}

void kl::context_holder::draw_mesh(const dx::buffer mesh, const D3D_PRIMITIVE_TOPOLOGY draw_type, const UINT stride) const
{
    set_draw_type(draw_type);
    bind_mesh(mesh, 0, 0, stride);
    draw(get_mesh_vertex_count(mesh, stride), 0);
}

// Views
void kl::context_holder::clear_target_view(dx::target_view view, const float4& color) const
{
    context_->ClearRenderTargetView(view.Get(), color);
}

void kl::context_holder::clear_depth_view(dx::depth_view view, const float depth, const UINT8 stencil) const
{
    context_->ClearDepthStencilView(view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void kl::context_holder::bind_target_depth_views(const std::vector<dx::target_view>& target_views, dx::depth_view depth_view) const
{
    std::vector<ID3D11RenderTargetView*> list_of_targets(target_views.size());
    for (size_t i = 0; i < list_of_targets.size(); i++) {
        list_of_targets[i] = target_views[i].Get();
    }

    context_->OMSetRenderTargets((UINT) target_views.size(), list_of_targets.data(), depth_view.Get());
}

void kl::context_holder::unbind_target_depth_views() const
{
    context_->OMSetRenderTargets(0, nullptr, nullptr);
}

void kl::context_holder::bind_shader_view_for_pixel_shader(const dx::shader_view view, const UINT slot) const
{
    context_->PSSetShaderResources(slot, 1, view.GetAddressOf());
}

void kl::context_holder::unbind_shader_view_for_pixel_shader(const UINT slot) const
{
    bind_shader_view_for_pixel_shader(nullptr, slot);
}

void kl::context_holder::bind_shader_view_for_compute_shader(const dx::shader_view view, const UINT slot) const
{
    context_->CSSetShaderResources(slot, 1, view.GetAddressOf());
}

void kl::context_holder::unbind_shader_view_for_compute_shader(const UINT slot) const
{
    bind_shader_view_for_compute_shader(nullptr, slot);
}

void kl::context_holder::bind_access_view_for_compute_shader(const dx::access_view view, const UINT slot, const UINT* initial_counts) const
{
    context_->CSSetUnorderedAccessViews(slot, 1, view.GetAddressOf(), initial_counts);
}

void kl::context_holder::unbind_access_view_for_compute_shader(const UINT slot) const
{
    bind_access_view_for_compute_shader(nullptr, slot);
}

// Shaders
void kl::context_holder::bind_input_layout(const dx::layout input_layout) const
{
    context_->IASetInputLayout(input_layout.Get());
}

void kl::context_holder::unbind_input_layout() const
{
    bind_input_layout(nullptr);
}

void kl::context_holder::bind_vertex_shader(const dx::vertex_shader shader) const
{
    context_->VSSetShader(shader.Get(), nullptr, 0);
}

void kl::context_holder::unbind_vertex_shader() const
{
    bind_vertex_shader(nullptr);
}

void kl::context_holder::bind_geometry_shader(const dx::geometry_shader shader) const
{
    context_->GSSetShader(shader.Get(), nullptr, 0);
}

void kl::context_holder::unbind_geometry_shader() const
{
    bind_geometry_shader(nullptr);
}

void kl::context_holder::bind_pixel_shader(const dx::pixel_shader shader) const
{
    context_->PSSetShader(shader.Get(), nullptr, 0);
}

void kl::context_holder::unbind_pixel_shader() const
{
    bind_pixel_shader(nullptr);
}

void kl::context_holder::bind_compute_shader(const dx::compute_shader shader) const
{
    context_->CSSetShader(shader.Get(), nullptr, 0);
}

void kl::context_holder::unbind_compute_shader() const
{
    bind_compute_shader(nullptr);
}

void kl::context_holder::bind_render_shaders(const render_shaders shaders) const
{
    bind_input_layout(shaders.input_layout);
    bind_vertex_shader(shaders.vertex_shader);
    bind_pixel_shader(shaders.pixel_shader);
}

void kl::context_holder::unbind_render_shaders() const
{
    bind_render_shaders({});
}

void kl::context_holder::dispatch_compute_shader(const UINT x, const UINT y, const UINT z) const
{
    context_->Dispatch(x, y, z);
}

void kl::context_holder::execute_compute_shader(const dx::compute_shader shader, const UINT x, const UINT y, const UINT z) const
{
    bind_compute_shader(shader);
    dispatch_compute_shader(x, y, z);
}

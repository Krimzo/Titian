#pragma once

#include "graphics/shaders/shaders.h"
#include "math/math.h"


namespace kl
{
struct ContextHolder : NoCopy
{
    ContextHolder() = default;

    void set_viewport_position( Int2 position ) const;
    Int2 viewport_position() const;

    void set_viewport_size( Int2 size ) const;
    Int2 viewport_size() const;

    void set_viewport_min_max( Float2 min_max ) const;
    Float2 viewport_min_max() const;

    void bind_raster_state( dx::RasterState const& state ) const;
    void unbind_raster_state() const;

    void bind_depth_state( dx::DepthState const& state, UINT stencil_ref = 0xFF ) const;
    void unbind_depth_state() const;

    void bind_sampler_state_for_pixel_shader( dx::SamplerState const& state, UINT slot ) const;
    void unbind_sampler_state_for_pixel_shader( UINT slot ) const;

    void bind_sampler_state_for_compute_shader( dx::SamplerState const& state, UINT slot ) const;
    void unbind_sampler_state_for_compute_shader( UINT slot ) const;

    void bind_blend_state( dx::BlendState const& state, float const* factor = nullptr, UINT mask = 0xFFFFFFFF ) const;
    void unbind_blend_state() const;

    void copy_resource( dx::Resource const& destination, dx::Resource const& source ) const;
    void copy_resource_region( dx::Resource const& destination, dx::Resource const& source, Int2 src_min, Int2 src_max, Int2 dst = { 0, 0 } ) const;

    void map_read_resource( dx::Resource const& resource, std::function<void( byte*, uint32_t )> const& func ) const;
    void map_write_resource( dx::Resource const& resource, std::function<void( byte*, uint32_t )> const& func, bool discard = true ) const;

    void read_from_buffer( void* cpu_buffer, dx::Buffer const& gpu_buffer, SIZE_T byte_size ) const;
    void write_to_buffer( dx::Buffer const& gpu_buffer, void const* cpu_buffer, SIZE_T byte_size, bool discard = true ) const;

    void read_from_texture( void* cpu_buffer, dx::Texture const& gpu_buffer, Int2 cpu_size, UINT element_size ) const;
    void write_to_texture( dx::Texture const& gpu_buffer, void const* cpu_buffer, Int2 cpu_size, UINT element_size, bool discard = true ) const;

    UINT buffer_size( dx::Buffer const& buffer ) const;
    Int2 texture_size( dx::Texture const& texture ) const;

    void bind_cb_for_vertex_shader( dx::Buffer const& buffer, UINT slot ) const;
    void unbind_cb_for_vertex_shader( UINT slot ) const;

    void bind_cb_for_geometry_shader( dx::Buffer const& buffer, UINT slot ) const;
    void unbind_cb_for_geometry_shader( UINT slot ) const;

    void bind_cb_for_pixel_shader( dx::Buffer const& buffer, UINT slot ) const;
    void unbind_cb_for_pixel_shader( UINT slot ) const;

    void bind_cb_for_compute_shader( dx::Buffer const& buffer, UINT slot ) const;
    void unbind_cb_for_compute_shader( UINT slot ) const;

    template<typename T>
    void set_cb_data( dx::Buffer const& buffer, T const& object ) const
    {
        write_to_resource( buffer, &object, sizeof( T ) );
    }

    UINT vertex_buffer_size( dx::Buffer const& buffer, UINT stride ) const;

    void bind_vertex_buffer( dx::Buffer const& buffer, UINT slot, UINT offset, UINT stride ) const;
    void unbind_vertex_buffer( UINT slot ) const;

    UINT index_buffer_size( dx::Buffer const& buffer ) const;

    void bind_index_buffer( dx::Buffer const& buffer, UINT offset ) const;
    void unbind_index_buffer( UINT slot ) const;

    void set_draw_type( D3D_PRIMITIVE_TOPOLOGY draw_type ) const;

    void draw( UINT vertex_count, UINT start_index ) const;
    void draw( dx::Buffer const& vertex_buffer, D3D_PRIMITIVE_TOPOLOGY draw_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, UINT stride = sizeof( Vertex ) ) const;

    void draw_indexed( UINT index_count, UINT start_index, INT base_vertex ) const;
    void draw_indexed( dx::Buffer const& vertex_buffer, dx::Buffer const& index_buffer, D3D_PRIMITIVE_TOPOLOGY draw_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, UINT stride = sizeof( Vertex ) ) const;

    void clear_target_view( dx::TargetView const& view, Float4 const& color = {} ) const;
    void clear_depth_view( dx::DepthView const& view, float depth = 1.0f, UINT8 stencil = 0xFF ) const;

    void bind_target_depth_view( dx::TargetView const& target_view, dx::DepthView const& depth_view ) const;
    void bind_target_depth_views( std::initializer_list<ID3D11RenderTargetView*> const& target_views, dx::DepthView const& depth_view ) const;
    void unbind_target_depth_views() const;

    void bind_shader_view_for_vertex_shader( dx::ShaderView const& view, UINT slot ) const;
    void unbind_shader_view_for_vertex_shader( UINT slot ) const;

    void bind_shader_view_for_pixel_shader( dx::ShaderView const& view, UINT slot ) const;
    void unbind_shader_view_for_pixel_shader( UINT slot ) const;

    void bind_shader_view_for_compute_shader( dx::ShaderView const& view, UINT slot ) const;
    void unbind_shader_view_for_compute_shader( UINT slot ) const;

    void bind_access_view_for_compute_shader( dx::AccessView const& view, UINT slot, UINT const* initial_counts = nullptr ) const;
    void unbind_access_view_for_compute_shader( UINT slot ) const;

    void bind_input_layout( dx::InputLayout const& input_layout ) const;
    void unbind_input_layout() const;

    void bind_vertex_shader( dx::VertexShader const& shader ) const;
    void unbind_vertex_shader() const;

    void bind_geometry_shader( dx::GeometryShader const& shader ) const;
    void unbind_geometry_shader() const;

    void bind_pixel_shader( dx::PixelShader const& shader ) const;
    void unbind_pixel_shader() const;

    void bind_compute_shader( dx::ComputeShader const& shader ) const;
    void unbind_compute_shader() const;

    void bind_shaders( Shaders const& shaders ) const;
    void unbind_shaders() const;

    void dispatch_compute_shader( UINT x, UINT y, UINT z ) const;
    void execute_compute_shader( dx::ComputeShader const& shader, UINT x, UINT y, UINT z ) const;

protected:
    dx::Context m_context;
};
}

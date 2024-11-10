#pragma once

#include "graphics/shaders/shader_compiler.h"
#include "media/image/image.h"


namespace kl
{
struct DeviceHolder : NoCopy
{
    DeviceHolder() = default;

    dx::RasterState create_raster_state( dx::RasterStateDescriptor const* descriptor ) const;
    dx::RasterState create_raster_state( bool wireframe, bool cull, bool cull_back = true ) const;

    dx::DepthState create_depth_state( dx::DepthStateDescriptor const* descriptor ) const;
    dx::DepthState create_depth_state( bool depth, bool stencil = false, bool mask = false ) const;

    dx::SamplerState create_sampler_state( dx::SamplerStateDescriptor const* descriptor ) const;
    dx::SamplerState create_sampler_state( bool linear, bool mirror ) const;

    dx::BlendState create_blend_state( dx::BlendStateDescriptor const* descriptor ) const;
    dx::BlendState create_blend_state( bool transparency ) const;

    dx::Buffer create_buffer( dx::BufferDescriptor const* descriptor, dx::SubresourceDescriptor const* subresource_data ) const;

    dx::Buffer create_vertex_buffer( void const* data, UINT byte_size ) const;
    dx::Buffer create_vertex_buffer( std::vector<Vertex> const& vertices ) const;
    dx::Buffer create_vertex_buffer( std::vector<Triangle> const& triangles ) const;
    dx::Buffer create_vertex_buffer( std::string_view const& filepath, bool flip_z = true ) const;

    dx::Buffer create_index_buffer( uint32_t const* data, UINT element_count ) const;
    dx::Buffer create_index_buffer( std::vector<uint32_t> const& indices ) const;

    dx::Buffer create_const_buffer( UINT byte_size ) const;
    dx::Buffer create_structured_buffer( void const* data, UINT element_count, UINT element_size, bool has_unordered_access = false, bool cpu_read = false ) const;
    dx::Buffer create_staging_buffer( dx::Buffer const& buffer, UINT byte_size ) const;

    static std::vector<Triangle> generate_screen_mesh();
    static std::vector<Triangle> generate_plane_mesh( float size, int complexity );
    static std::vector<Triangle> generate_cube_mesh( float size );
    static std::vector<Triangle> generate_sphere_mesh( float radius, int complexity, bool smooth );
    static std::vector<Triangle> generate_capsule_mesh( float radius, float height, int sectors, int rings );

    dx::Buffer create_screen_mesh() const;
    dx::Buffer create_plane_mesh( float size, int complexity ) const;
    dx::Buffer create_cube_mesh( float size ) const;
    dx::Buffer create_sphere_mesh( float radius, int complexity, bool smooth ) const;
    dx::Buffer create_capsule_mesh( float radius, float height, int sectors, int rings ) const;

    dx::Texture create_texture( dx::TextureDescriptor const* descriptor, dx::SubresourceDescriptor const* subresource_data ) const;
    dx::Texture create_texture( Image const& image, bool has_unordered_access = false, bool is_target = false ) const;
    dx::Texture create_cube_texture( Image const& right, Image const& left, Image const& top, Image const& bottom, Image const& front, Image const& back ) const;
    dx::Texture create_staging_texture( dx::Texture const& texture, Int2 size = {} ) const;
    dx::Texture create_target_texture( Int2 size ) const;

    dx::TargetView create_target_view( dx::Resource const& resource, dx::TargetViewDescriptor const* descriptor ) const;
    dx::DepthView create_depth_view( dx::Resource const& resource, dx::DepthViewDescriptor const* descriptor ) const;
    dx::ShaderView create_shader_view( dx::Resource const& resource, dx::ShaderViewDescriptor const* descriptor ) const;
    dx::AccessView create_access_view( dx::Resource const& resource, dx::AccessViewDescriptor const* descriptor ) const;

    dx::InputLayout create_input_layout( CompiledShader const& compiled_shader, std::vector<dx::LayoutDescriptor> const& descriptors = {} ) const;

    dx::VertexShader create_vertex_shader( CompiledShader const& compiled_shader ) const;
    dx::GeometryShader create_geometry_shader( CompiledShader const& compiled_shader ) const;
    dx::PixelShader create_pixel_shader( CompiledShader const& compiled_shader ) const;
    dx::ComputeShader create_compute_shader( CompiledShader const& compiled_shader ) const;

protected:
    dx::Device m_device;
};
}

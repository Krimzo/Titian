#pragma once

#include "graphics/shaders/shader_compiler.h"
#include "media/image/image.h"


namespace kl {
    struct DeviceHolder : NoCopy
    {
        DeviceHolder() = default;

        dx::RasterState create_raster_state(const dx::RasterStateDescriptor* descriptor) const;
        dx::RasterState create_raster_state(bool wireframe, bool cull, bool cull_back = true) const;

        dx::DepthState create_depth_state(const dx::DepthStateDescriptor* descriptor) const;
        dx::DepthState create_depth_state(bool depth, bool stencil = false, bool mask = false) const;

        dx::SamplerState create_sampler_state(const dx::SamplerStateDescriptor* descriptor) const;
        dx::SamplerState create_sampler_state(bool linear, bool mirror) const;

        dx::BlendState create_blend_state(const dx::BlendStateDescriptor* descriptor) const;
        dx::BlendState create_blend_state(bool transparency) const;

        dx::Buffer create_buffer(const dx::BufferDescriptor* descriptor, const dx::SubresourceDescriptor* subresource_data) const;
        
        dx::Buffer create_vertex_buffer(const void* data, UINT byte_size) const;
        dx::Buffer create_vertex_buffer(const std::vector<Vertex>& vertices) const;
        dx::Buffer create_vertex_buffer(const std::vector<Triangle>& triangles) const;
        dx::Buffer create_vertex_buffer(const std::string_view& filepath, bool flip_z = true) const;

        dx::Buffer create_index_buffer(const uint32_t* data, UINT element_count) const;
        dx::Buffer create_index_buffer(const std::vector<uint32_t>& indices) const;

        dx::Buffer create_const_buffer(UINT byte_size) const;
        dx::Buffer create_structured_buffer(const void* data, UINT element_count, UINT element_size, bool has_unordered_access = false, bool cpu_read = false) const;
        dx::Buffer create_staging_buffer(const dx::Buffer& buffer, UINT byte_size) const;

        static std::vector<Triangle> generate_screen_mesh();
        static std::vector<Triangle> generate_plane_mesh(float size, int complexity);
        static std::vector<Triangle> generate_cube_mesh(float size);
        static std::vector<Triangle> generate_sphere_mesh(float radius, int complexity, bool smooth);
        static std::vector<Triangle> generate_capsule_mesh(float radius, float height, int sectors, int rings);

        dx::Buffer create_screen_mesh() const;
        dx::Buffer create_plane_mesh(float size, int complexity) const;
        dx::Buffer create_cube_mesh(float size) const;
        dx::Buffer create_sphere_mesh(float radius, int complexity, bool smooth) const;
        dx::Buffer create_capsule_mesh(float radius, float height, int sectors, int rings) const;

        dx::Texture create_texture(const dx::TextureDescriptor* descriptor, const dx::SubresourceDescriptor* subresource_data) const;
        dx::Texture create_texture(const Image& image, bool has_unordered_access = false, bool is_target = false) const;
        dx::Texture create_cube_texture(const Image& right, const Image& left, const Image& top, const Image& bottom, const Image& front, const Image& back) const;
        dx::Texture create_staging_texture(const dx::Texture& texture, Int2 size = {}) const;
        dx::Texture create_target_texture(Int2 size) const;

        dx::TargetView create_target_view(const dx::Resource& resource, const dx::TargetViewDescriptor* descriptor) const;
        dx::DepthView create_depth_view(const dx::Resource& resource, const dx::DepthViewDescriptor* descriptor) const;
        dx::ShaderView create_shader_view(const dx::Resource& resource, const dx::ShaderViewDescriptor* descriptor) const;
        dx::AccessView create_access_view(const dx::Resource& resource, const dx::AccessViewDescriptor* descriptor) const;

        dx::InputLayout create_input_layout(const CompiledShader& compiled_shader, const std::vector<dx::LayoutDescriptor>& descriptors = {}) const;

        dx::VertexShader create_vertex_shader(const CompiledShader& compiled_shader) const;
        dx::GeometryShader create_geometry_shader(const CompiledShader& compiled_shader) const;
        dx::PixelShader create_pixel_shader(const CompiledShader& compiled_shader) const;
        dx::ComputeShader create_compute_shader(const CompiledShader& compiled_shader) const;

    protected:
        dx::Device m_device;
    };
}

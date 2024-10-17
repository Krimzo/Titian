#pragma once

#include "graphics/shaders/shaders.h"
#include "math/math.h"


namespace kl {
    struct ContextHolder : NoCopy
    {
        ContextHolder() = default;

        void set_viewport_position(Int2 position) const;
        Int2 viewport_position() const;

        void set_viewport_size(Int2 size) const;
        Int2 viewport_size() const;

        void set_viewport_min_max(Float2 min_max) const;
        Float2 viewport_min_max() const;

        void bind_raster_state(const dx::RasterState& state) const;
        void unbind_raster_state() const;

        void bind_depth_state(const dx::DepthState& state, UINT stencil_ref = 0xFF) const;
        void unbind_depth_state() const;

        void bind_sampler_state_for_pixel_shader(const dx::SamplerState& state, UINT slot) const;
        void unbind_sampler_state_for_pixel_shader(UINT slot) const;

        void bind_sampler_state_for_compute_shader(const dx::SamplerState& state, UINT slot) const;
        void unbind_sampler_state_for_compute_shader(UINT slot) const;

        void bind_blend_state(const dx::BlendState& state, const float* factor = nullptr, UINT mask = 0xFFFFFFFF) const;
        void unbind_blend_state() const;

        void copy_resource(const dx::Resource& destination, const dx::Resource& source) const;
        void copy_resource_region(const dx::Resource& destination, const dx::Resource& source, Int2 src_min, Int2 src_max, Int2 dst = { 0, 0 }) const;

        void map_read_resource(const dx::Resource& resource, const std::function<void(const byte*, uint32_t)>& func) const;
        void map_write_resource(const dx::Resource& resource, const std::function<void(byte*, uint32_t)>& func, bool discard = true) const;

        void read_from_buffer(void* cpu_buffer, const dx::Buffer& gpu_buffer, SIZE_T byte_size) const;
        void write_to_buffer(const dx::Buffer& gpu_buffer, const void* cpu_buffer, SIZE_T byte_size, bool discard = true) const;

        void read_from_texture(void* cpu_buffer, const dx::Texture& gpu_buffer, Int2 cpu_size, UINT element_size) const;
        void write_to_texture(const dx::Texture& gpu_buffer, const void* cpu_buffer, Int2 cpu_size, UINT element_size, bool discard = true) const;

        UINT buffer_size(const dx::Buffer& buffer) const;
        Int2 texture_size(const dx::Texture& texture) const;

        void bind_cb_for_vertex_shader(const dx::Buffer& buffer, UINT slot) const;
        void unbind_cb_for_vertex_shader(UINT slot) const;

        void bind_cb_for_geometry_shader(const dx::Buffer& buffer, UINT slot) const;
        void unbind_cb_for_geometry_shader(UINT slot) const;

        void bind_cb_for_pixel_shader(const dx::Buffer& buffer, UINT slot) const;
        void unbind_cb_for_pixel_shader(UINT slot) const;

        void bind_cb_for_compute_shader(const dx::Buffer& buffer, UINT slot) const;
        void unbind_cb_for_compute_shader(UINT slot) const;

        template<typename T>
        void set_cb_data(const dx::Buffer& buffer, const T& object) const
        {
            write_to_resource(buffer, &object, sizeof(T));
        }

        UINT vertex_buffer_size(const dx::Buffer& buffer, UINT stride) const;

        void bind_vertex_buffer(const dx::Buffer& buffer, UINT slot, UINT offset, UINT stride) const;
        void unbind_vertex_buffer(UINT slot) const;

        UINT index_buffer_size(const dx::Buffer& buffer) const;

        void bind_index_buffer(const dx::Buffer& buffer, UINT offset) const;
        void unbind_index_buffer(UINT slot) const;

        void set_draw_type(D3D_PRIMITIVE_TOPOLOGY draw_type) const;

        void draw(UINT vertex_count, UINT start_index) const;
        void draw(const dx::Buffer& vertex_buffer, D3D_PRIMITIVE_TOPOLOGY draw_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, UINT stride = sizeof(Vertex)) const;

        void draw_indexed(UINT index_count, UINT start_index, INT base_vertex) const;
        void draw_indexed(const dx::Buffer& vertex_buffer, const dx::Buffer& index_buffer, D3D_PRIMITIVE_TOPOLOGY draw_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, UINT stride = sizeof(Vertex)) const;

        void clear_target_view(const dx::TargetView& view, const Float4& color = {}) const;
        void clear_depth_view(const dx::DepthView& view, float depth = 1.0f, UINT8 stencil = 0xFF) const;

        void bind_target_depth_view(const dx::TargetView& target_view, const dx::DepthView& depth_view) const;
        void bind_target_depth_views(const std::initializer_list<ID3D11RenderTargetView*>& target_views, const dx::DepthView& depth_view) const;
        void unbind_target_depth_views() const;

        void bind_shader_view_for_vertex_shader(const dx::ShaderView& view, UINT slot) const;
        void unbind_shader_view_for_vertex_shader(UINT slot) const;

        void bind_shader_view_for_pixel_shader(const dx::ShaderView& view, UINT slot) const;
        void unbind_shader_view_for_pixel_shader(UINT slot) const;

        void bind_shader_view_for_compute_shader(const dx::ShaderView& view, UINT slot) const;
        void unbind_shader_view_for_compute_shader(UINT slot) const;

        void bind_access_view_for_compute_shader(const dx::AccessView& view, UINT slot, const UINT* initial_counts = nullptr) const;
        void unbind_access_view_for_compute_shader(UINT slot) const;

        void bind_input_layout(const dx::InputLayout& input_layout) const;
        void unbind_input_layout() const;

        void bind_vertex_shader(const dx::VertexShader& shader) const;
        void unbind_vertex_shader() const;

        void bind_geometry_shader(const dx::GeometryShader& shader) const;
        void unbind_geometry_shader() const;

        void bind_pixel_shader(const dx::PixelShader& shader) const;
        void unbind_pixel_shader() const;

        void bind_compute_shader(const dx::ComputeShader& shader) const;
        void unbind_compute_shader() const;

        void bind_shaders(const Shaders& shaders) const;
        void unbind_shaders() const;

        void dispatch_compute_shader(UINT x, UINT y, UINT z) const;
        void execute_compute_shader(const dx::ComputeShader& shader, UINT x, UINT y, UINT z) const;

    protected:
        dx::Context m_context;
    };
}

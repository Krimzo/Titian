#pragma once

#include "graphics/shaders/render_shaders.h"
#include "math/math.h"


namespace kl {
    class context_holder
    {
    protected:
        dx::context context_ = nullptr;

    public:
        context_holder() = default;

        context_holder(const context_holder&) = delete;
        context_holder(const context_holder&&) = delete;

        void operator=(const context_holder&) = delete;
        void operator=(const context_holder&&) = delete;

        // Viewport
        void set_viewport_position(const int2& position) const;
        int2 viewport_position() const;

        void set_viewport_size(const int2& size) const;
        int2 viewport_size() const;

        void set_viewport_min_max(const float2& min_max) const;
        float2 viewport_min_max() const;

        // States
        void bind_raster_state(const dx::raster_state& state) const;
        void unbind_rater_state() const;

        void bind_depth_state(const dx::depth_state& state, UINT stencil_ref = 0xFF) const;
        void unbind_depth_state() const;

        void bind_sampler_state_for_pixel_shader(const dx::sampler_state& state, UINT slot) const;
        void unbind_sampler_state_for_pixel_shader(UINT slot) const;

        void bind_sampler_state_for_compute_shader(const dx::sampler_state& state, UINT slot) const;
        void unbind_sampler_state_for_compute_shader(UINT slot) const;

        void bind_blend_state(const dx::blend_state& state, const float* factor = nullptr, UINT mask = 0xFFFFFFFF) const;
        void unbind_blend_state() const;

        // Resources
        void copy_resource(const dx::resource& destination, const dx::resource& source) const;
        void read_from_resource(void* cpu_buffer, const dx::resource& gpu_buffer, SIZE_T byte_size) const;
        void write_to_resource(const dx::resource& gpu_buffer, const void* cpu_buffer, SIZE_T byte_size, bool discard = true) const;

        // Buffers
        UINT buffer_size(const dx::buffer& buffer) const;

        // Const buffers
        void bind_cb_for_vertex_shader(const dx::buffer& buffer, UINT slot) const;
        void unbind_cb_for_vertex_shader(UINT slot) const;

        void bind_cb_for_geometry_shader(const dx::buffer& buffer, UINT slot) const;
        void unbind_cb_for_geometry_shader(UINT slot) const;

        void bind_cb_for_pixel_shader(const dx::buffer& buffer, UINT slot) const;
        void unbind_cb_for_pixel_shader(UINT slot) const;

        void bind_cb_for_compute_shader(const dx::buffer& buffer, UINT slot) const;
        void unbind_cb_for_compute_shader(UINT slot) const;

        template<typename T>
        void set_cb_data(const dx::buffer& buffer, const T& object) const
        {
            write_to_resource(buffer, &object, sizeof(T));
        }

        // Vertex buffers
        UINT vertex_buffer_size(const dx::buffer& buffer, UINT stride) const;

        void bind_vertex_buffer(const dx::buffer& buffer, UINT slot, UINT offset, UINT stride) const;
        void unbind_vertex_buffer(UINT slot) const;

        // Index buffers
        UINT index_buffer_size(const dx::buffer& buffer) const;

        void bind_index_buffer(const dx::buffer& buffer, UINT offset) const;
        void unbind_index_buffer(UINT slot) const;

        // Draw
        void set_draw_type(D3D_PRIMITIVE_TOPOLOGY draw_type) const;

        void draw(UINT vertex_count, UINT start_index) const;
        void draw(const dx::buffer& vertex_buffer, D3D_PRIMITIVE_TOPOLOGY draw_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, UINT stride = sizeof(vertex)) const;

        void draw_indexed(UINT index_count, UINT start_index, INT base_vertex) const;
        void draw_indexed(const dx::buffer& vertex_buffer, const dx::buffer& index_buffer, D3D_PRIMITIVE_TOPOLOGY draw_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, UINT stride = sizeof(vertex)) const;

        // Views
        void clear_target_view(const dx::target_view& view, const float4& color = {}) const;
        void clear_depth_view(const dx::depth_view& view, float depth = 1.0f, UINT8 stencil = 0xFF) const;

        void bind_target_depth_views(const std::vector<dx::target_view>& target_views, const dx::depth_view& depth_view) const;
        void unbind_target_depth_views() const;

        void bind_shader_view_for_pixel_shader(const dx::shader_view& view, UINT slot) const;
        void unbind_shader_view_for_pixel_shader(UINT slot) const;

        void bind_shader_view_for_compute_shader(const dx::shader_view& view, UINT slot) const;
        void unbind_shader_view_for_compute_shader(UINT slot) const;

        void bind_access_view_for_compute_shader(const dx::access_view& view, UINT slot, const UINT* initial_counts = nullptr) const;
        void unbind_access_view_for_compute_shader(UINT slot) const;

        // Shaders
        void bind_input_layout(const dx::layout& input_layout) const;
        void unbind_input_layout() const;

        void bind_vertex_shader(const dx::vertex_shader& shader) const;
        void unbind_vertex_shader() const;

        void bind_geometry_shader(const dx::geometry_shader& shader) const;
        void unbind_geometry_shader() const;

        void bind_pixel_shader(const dx::pixel_shader& shader) const;
        void unbind_pixel_shader() const;

        void bind_compute_shader(const dx::compute_shader& shader) const;
        void unbind_compute_shader() const;

        void bind_render_shaders(const render_shaders& shaders) const;
        void unbind_render_shaders() const;

        void dispatch_compute_shader(UINT x, UINT y, UINT z) const;
        void execute_compute_shader(const dx::compute_shader& shader, UINT x, UINT y, UINT z) const;
    };
}

#pragma once

#include "graphics/shaders/compiled_shader.h"
#include "graphics/shaders/render_shaders.h"
#include "media/media.h"


namespace kl {
    using mesh_data = std::vector<vertex>;

    class device_holder
    {
    protected:
        dx::device device_ = nullptr;

    public:
        // Creation
        device_holder();
        virtual ~device_holder();

        device_holder(const device_holder&) = delete;
        device_holder(const device_holder&&) = delete;

        void operator=(const device_holder&) = delete;
        void operator=(const device_holder&&) = delete;

        // States
        dx::raster_state create_raster_state(const dx::raster_state_descriptor* descriptor) const;
        dx::raster_state create_raster_state(bool wireframe, bool cull, bool cull_back = true) const;

        dx::depth_state create_depth_state(const dx::depth_state_descriptor* descriptor) const;
        dx::depth_state create_depth_state(bool depth, bool stencil = false, bool mask = false) const;

        dx::sampler_state create_sampler_state(const dx::sampler_state_descriptor* descriptor) const;
        dx::sampler_state create_sampler_state(bool linear, bool mirror) const;

        // Buffers
        dx::buffer create_buffer(const dx::buffer_descriptor* descriptor, const dx::subresource_descriptor* subresource_data) const;
        dx::buffer create_structured_buffer(const void* data, UINT element_count, UINT element_size, bool has_unordered_access = false, bool cpu_read = false) const;
        dx::buffer create_staging_buffer(dx::buffer buffer, UINT byte_size) const;

        // Const buffers
        dx::buffer create_const_buffer(UINT byte_size) const;

        // Meshes
        dx::buffer create_mesh(const mesh_data& vertices) const;
        dx::buffer create_mesh(const std::string& filepath, bool flip_z) const;
        dx::buffer create_plane_mesh(float size, size_t num_of_points) const;
        dx::buffer create_screen_mesh() const;

        // Textures
        dx::texture create_texture(const dx::texture_descriptor* descriptor, const dx::subresource_descriptor* subresource_data) const;
        dx::texture create_texture(const image& image, bool has_unordered_access = false, bool is_target = false) const;
        dx::texture create_cube_texture(const image& front, const image& back, const image& left, const image& right, const image& top, const image& bottom) const;
        dx::texture create_staging_texture(dx::texture texture, const int2& size) const;

        // Views
        dx::target_view create_target_view(dx::resource resource, const dx::target_view_descriptor* descriptor) const;
        dx::depth_view create_depth_view(dx::resource resource, const dx::depth_view_descriptor* descriptor) const;
        dx::shader_view create_shader_view(dx::resource resource, const dx::shader_view_descriptor* descriptor) const;
        dx::access_view create_access_view(dx::resource resource, const dx::access_view_descriptor* descriptor) const;

        // Shaders
        dx::layout create_input_layout(const compiled_shader& compiled_shader, const std::vector<dx::layout_descriptor>& descriptors = {}) const;

        dx::vertex_shader create_vertex_shader(const compiled_shader& compiled_shader) const;
        dx::geometry_shader create_geometry_shader(const compiled_shader& compiled_shader) const;
        dx::pixel_shader create_pixel_shader(const compiled_shader& compiled_shader) const;
        dx::compute_shader create_compute_shader(const compiled_shader& compiled_shader) const;

        render_shaders create_render_shaders(const compiled_shader& compiled_vertex_shader, const compiled_shader& compiled_pixel_shader) const;
    };
}

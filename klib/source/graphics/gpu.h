#pragma once

#include "graphics/dx_types.h"
#include "render/vertex.h"
#include "media/image.h"

#include <set>


namespace kl {
    struct shaders
    {
        dx::vertex_shader vertex_shader = nullptr;
        dx::pixel_shader pixel_shader = nullptr;
        dx::layout layout = nullptr;

        explicit operator bool() const
        {
            return vertex_shader && pixel_shader && layout;
        }
    };
}

namespace kl {
    enum class gpu_creation_type
    {
        none = 0,
        render,
        compute
    };
}

namespace kl {
    inline constexpr int cbuffer_predefined_size = 64;

    class gpu
    {
        gpu_creation_type creation_type_ = gpu_creation_type::none;

        dx::device device_ = nullptr;
        dx::context context_ = nullptr;
        dx::chain chain_ = nullptr;

        dx::target_view frame_buffer_ = nullptr;
        dx::depth_view depth_buffer_ = nullptr;

        std::set<IUnknown*> children_ = {};

        dx::buffer vertex_const_buffers_[cbuffer_predefined_size] = {};
        dx::buffer pixel_const_buffers_[cbuffer_predefined_size] = {};
        dx::buffer compute_const_buffers_[cbuffer_predefined_size] = {};

    public:
        gpu();
        explicit gpu(HWND window);
        ~gpu();

        gpu(const gpu&) = delete;
        gpu(const gpu&&) = delete;
        
        void operator=(const gpu&) = delete;
        void operator=(const gpu&&) = delete;

        [[nodiscard]] gpu_creation_type type() const;
        [[nodiscard]] dx::device device() const;
        [[nodiscard]] dx::context context() const;
        [[nodiscard]] dx::chain chain() const;

        void set_viewport(const int2& size) const;
        void set_viewport(const int2& position, const int2& size) const;

        void unbind_all_targets() const;
        void bind_internal_targets() const;
        void bind_targets(const std::vector<dx::target_view>& targets, dx::depth_view depth_view = nullptr) const;
        void bind_targets_with_internal(const std::vector<dx::target_view>& additional_targets,
                                     dx::depth_view depth_view = nullptr) const;

        void resize_internal(const int2& size);

        void clear_internal_color(const float4& color) const;
        void clear_internal_depth(float value) const;
        void clear_internal() const;

        void swap_buffers(bool v_sync) const;

        void copy_resource(dx::resource destination, dx::resource source) const;
        void read_from_resource(void* cpu_buffer, dx::resource cpu_read_resource, int byte_size) const;
        void write_to_resource(dx::resource cpu_write_resource, const void* data, int byte_size, bool discard = true) const;

        // Raster state
        dx::raster_state new_raster_state(const dx::raster_state_descriptor* descriptor);
        dx::raster_state new_raster_state(bool wireframe, bool cull, bool cull_back = true);
        void bind_raster_state(dx::raster_state state) const;

        // Depth/stencil state
        dx::depth_state new_depth_state(const dx::depth_state_descriptor* descriptor);
        dx::depth_state new_depth_state(bool depth, bool stencil, bool mask);
        void bind_depth_state(dx::depth_state state) const;

        // Shaders
        dx::vertex_shader new_vertex_shader(const std::string& source, dx::layout* out_layout = nullptr,
                                         const std::vector<dx::layout_descriptor>& descriptors = {});
        dx::pixel_shader new_pixel_shader(const std::string& source);
        dx::geometry_shader new_geometry_shader(const std::string& source);
        dx::compute_shader new_compute_shader(const std::string& source);

        shaders new_shaders(const std::string& vertex_source, const std::string& pixel_source,
                           const std::vector<dx::layout_descriptor>& descriptors = {});
        shaders new_shaders(const std::string& source, const std::vector<dx::layout_descriptor>& descriptors = {});

        void bind_vertex_shader(dx::vertex_shader shader) const;
        void bind_pixel_shader(dx::pixel_shader shader) const;
        void bind_geometry_shader(dx::geometry_shader shader) const;
        void bind_compute_shader(dx::compute_shader shader) const;

        void bind_layout(dx::layout layout) const;

        void bind_shaders(const shaders& shaders) const;

        void dispatch_compute_shader(const int3& size) const;
        void execute_compute_shader(dx::compute_shader shader, const int3& size) const;

        void destroy(const shaders& shaders);

        // Buffer
        dx::buffer new_buffer(const dx::buffer_descriptor* descriptor, const dx::subresource_descriptor* subresource_data = nullptr);
        dx::buffer new_structured_buffer(const void* data, int element_count, int element_size,
                                       bool has_unordered_access = false, bool cpu_read = false);
        dx::buffer new_staging_buffer(dx::buffer buffer, int byte_size = 0);

        // Constant buffer
        dx::buffer new_const_buffer(int byte_size);

        void set_const_buffer_data(dx::buffer cbuffer, const void* data) const;

        void bind_vertex_const_buffer(dx::buffer cbuffer, int slot) const;
        void bind_pixel_const_buffer(dx::buffer cbuffer, int slot) const;
        void bind_compute_const_buffer(dx::buffer cbuffer, int slot) const;

        template <typename T>
        bool set_vertex_const_buffer(const T& data, const int slot = 0)
        {
            if (sizeof(T) > (cbuffer_predefined_size * 16) || sizeof(T) % 16) {
                return false;
            }
            const dx::buffer buffer = vertex_const_buffers_[sizeof(T) / 16 - 1];
            bind_vertex_const_buffer(buffer, slot);
            set_const_buffer_data(buffer, &data);
            return true;
        }

        template <typename T>
        bool set_pixel_const_buffer(const T& data, const int slot = 0)
        {
            if (sizeof(T) > (cbuffer_predefined_size * 16) || sizeof(T) % 16) {
                return false;
            }
            const dx::buffer buffer = pixel_const_buffers_[sizeof(T) / 16 - 1];
            bind_pixel_const_buffer(buffer, slot);
            set_const_buffer_data(buffer, &data);
            return true;
        }

        template <typename T>
        bool set_compute_const_buffer(const T& data, const int slot = 0)
        {
            if (sizeof(T) > (cbuffer_predefined_size * 16) || sizeof(T) % 16) {
                return false;
            }
            const dx::buffer buffer = compute_const_buffers_[sizeof(T) / 16 - 1];
            bind_compute_const_buffer(buffer, slot);
            set_const_buffer_data(buffer, &data);
            return true;
        }

        // Vertex buffer
        dx::buffer new_vertex_buffer(const std::vector<vertex>& vertex_data);
        dx::buffer new_vertex_buffer(const std::string& filepath, bool flip_z);

        dx::buffer generate_screen_mesh();
        dx::buffer generate_plane_mesh(float size, int num_of_points);

        void draw_vertex_buffer(dx::buffer buffer) const;

        // Sampler
        dx::sampler_state new_sampler_state(const dx::sampler_state_descriptor* descriptor);
        dx::sampler_state new_sampler_state(bool linear, bool mirror);

        void bind_sampler_state(dx::sampler_state state, int slot) const;

        // Texture
        dx::texture get_back_buffer();

        dx::texture new_texture(const dx::texture_descriptor* descriptor, const dx::subresource_descriptor* subresource_data = nullptr);

        dx::texture new_texture(const image& image, bool has_unordered_access = false, bool is_target = false);
        dx::texture new_texture(const image& front, const image& back, const image& left, const image& right,
                                const image& top, const image& bottom);

        dx::texture new_staging_texture(dx::texture texture, const int2& size = {});

        // Render target view
        dx::target_view new_target_view(dx::texture texture, const dx::target_view_descriptor* descriptor = nullptr);

        void clear_target_view(dx::target_view view, const float4& values) const;

        // Depth stencil view
        dx::depth_view new_depth_view(dx::texture texture, const dx::depth_view_descriptor* descriptor = nullptr);

        void clear_depth_view(dx::depth_view view, float depth = 1.0f, uint8_t stencil = 0) const;

        // Shader resource view
        dx::shader_view new_shader_view(dx::resource resource, const dx::shader_view_descriptor* descriptor = nullptr);

        void bind_pixel_shader_view(dx::shader_view view, int slot) const;
        void bind_compute_shader_view(dx::shader_view view, int slot) const;

        // Shader access view
        dx::access_view new_access_view(dx::resource resource, const dx::access_view_descriptor* descriptor = nullptr);

        void bind_compute_access_view(dx::access_view view, int slot, const int* initial_counts = nullptr) const;

        // Deletes child instance
        void destroy(IUnknown* child);
    };
}

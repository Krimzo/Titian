#pragma once

#include "graphics/device_holder/device_holder.h"
#include "graphics/context_holder/context_holder.h"
#include "graphics/shaders/shader_compiler.h"


namespace kl {
    enum class gpu_creation_type
    {
        none = 0,
        render = 1,
        compute = 2,
    };

    class gpu : public device_holder, public context_holder, public shader_compiler
    {
        dx::chain chain_ = nullptr;

        dx::target_view target_view_ = nullptr;
        dx::depth_view depth_view_ = nullptr;

    public:
        const gpu_creation_type creation_type = gpu_creation_type::none;

        // Creation
        gpu();
        gpu(HWND window);
        ~gpu();

        gpu(const gpu&) = delete;
        gpu(const gpu&&) = delete;

        void operator=(const gpu&) = delete;
        void operator=(const gpu&&) = delete;

        // Get
        dx::device device() const;
        dx::context context() const;
        dx::chain chain() const;

        dx::target_view get_internal_target() const;
        dx::depth_view get_internal_depth() const;

        // Chain
        dx::texture get_back_buffer() const;
        void swap_buffers(bool v_sync) const;

        bool in_fullscreen() const;
        void set_fullscreen(bool enabled) const;

        // Internal buffers
        void clear_internal_color(const float4& color = {}) const;
        void clear_internal_depth(float depth = 1.0f, UINT8 stencil = 0xFF) const;
        void clear_internal(const float4& color = {}) const;

        void resize_internal(const int2& size);
        void resize_to_window(HWND window);

        void bind_internal_views() const;

        // Shader helper
        dx::vertex_shader create_vertex_shader(const std::string& shader_source);
        dx::geometry_shader create_geometry_shader(const std::string& shader_source);
        dx::pixel_shader create_pixel_shader(const std::string& shader_source);
        dx::compute_shader create_compute_shader(const std::string& shader_source);

        render_shaders create_render_shaders(const std::string& shader_sources);
    };
}

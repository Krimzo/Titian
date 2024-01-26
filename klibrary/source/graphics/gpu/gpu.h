#pragma once

#include "graphics/gpu/device_holder.h"
#include "graphics/gpu/context_holder.h"
#include "graphics/shaders/shader_compiler.h"


namespace kl {
    enum class GPUCreationType
    {
        NONE = 0,
        RENDER,
        COMPUTE,
    };
}

namespace kl {
    class GPU : public DeviceHolder, public ContextHolder, public ShaderCompiler
    {
    public:
        static inline constexpr UINT BUFFER_COUNT = 2;

    private:
        dx::Chain m_chain = nullptr;
        dx::TargetView m_target_views[BUFFER_COUNT] = {};
        dx::DepthView m_depth_views[BUFFER_COUNT] = {};

    public:
        const GPUCreationType creation_type = GPUCreationType::NONE;

        // Creation
        GPU(bool debug = false, bool single_threaded = true);
        GPU(HWND window, bool debug = false, bool single_threaded = true);
        virtual ~GPU();

        GPU(const GPU&) = delete;
        GPU(const GPU&&) = delete;

        void operator=(const GPU&) = delete;
        void operator=(const GPU&&) = delete;

        // Get
        dx::Device device() const;
        dx::Context context() const;
        dx::Chain chain() const;

        dx::TargetView internal_target() const;
        dx::DepthView internal_depth() const;

        // Chain
        dx::Texture back_buffer() const;
        void swap_buffers(bool v_sync) const;

        bool in_fullscreen() const;
        void set_fullscreen(bool enabled) const;

        // Internal buffers
        void clear_internal_color(const Float4& color = {}) const;
        void clear_internal_depth(float depth = 1.0f, UINT8 stencil = 0xFF) const;
        void clear_internal(const Float4& color = {}) const;

        void resize_internal(const Int2& size);
        void resize_to_window(HWND window);

        void bind_internal_views() const;

        // Shader helper
        ShaderHolder<dx::VertexShader> create_vertex_shader(const std::string& shader_source);
        ShaderHolder<dx::GeometryShader> create_geometry_shader(const std::string& shader_source);
        ShaderHolder<dx::PixelShader> create_pixel_shader(const std::string& shader_source);
        ShaderHolder<dx::ComputeShader> create_compute_shader(const std::string& shader_source);

        RenderShaders create_render_shaders(const std::string& shader_sources);
    };
}

#pragma once

#include "graphics/gpu/device_holder.h"
#include "graphics/gpu/context_holder.h"
#include "graphics/text/text_raster.h"
#include "graphics/shaders/shader_compiler.h"


namespace kl {
    struct GPU : DeviceHolder, ContextHolder, ShaderCompiler, TextRaster
    {
		inline GPU(HWND window)
		    : GPU(window, IS_DEBUG, false)
        {}

        virtual ~GPU() noexcept
        {
            if (m_chain) set_fullscreen(false);
        }

        dx::Device device() const;
        dx::Context context() const;
        dx::Chain chain() const;

        UINT back_index() const;

        dx::Texture target_buffer(UINT index) const;
        dx::Texture depth_buffer(UINT index) const;
        dx::TargetView target_view(UINT index) const;
        dx::DepthView depth_view(UINT index) const;

        dx::Texture back_target_buffer() const;
        dx::Texture back_depth_buffer() const;
        dx::TargetView back_target_view() const;
        dx::DepthView back_depth_view() const;

        void swap_buffers(bool v_sync) const;
        void set_fullscreen(bool enabled) const;
        bool in_fullscreen() const;

        void clear_internal_color(const Float4& color = {}) const;
        void clear_internal_depth(float depth = 1.0f, UINT8 stencil = 0xFF) const;
        void clear_internal(const Float4& color = {}) const;

        void resize_internal(Int2 size, DXGI_FORMAT depth_format = DXGI_FORMAT_D32_FLOAT);
        void resize_to_window(HWND window);

        void bind_internal_views() const;

        VertexShader create_vertex_shader(const std::string_view& shader_source) const;
        PixelShader create_pixel_shader(const std::string_view& shader_source) const;
        GeometryShader create_geometry_shader(const std::string_view& shader_source) const;
        ComputeShader create_compute_shader(const std::string_view& shader_source) const;
        Shaders create_shaders(const std::string_view& shader_sources, const std::vector<dx::LayoutDescriptor>& descriptors = {}) const;

        void draw_text() const;

    protected:
        GPU(HWND window, bool debug, bool video_support);

    private:
        dx::Chain m_chain;
        dx::Texture m_depth_textures[GPU_BUFFER_COUNT] = {};
        dx::TargetView m_target_views[GPU_BUFFER_COUNT] = {};
        dx::DepthView m_depth_views[GPU_BUFFER_COUNT] = {};
    };
}

namespace kl {
    struct VideoGPU : GPU
    {
        inline VideoGPU()
            : GPU(nullptr, IS_DEBUG, true)
        {}
    };
}

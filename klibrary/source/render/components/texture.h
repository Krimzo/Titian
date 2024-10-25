#pragma once

#include "media/media.h"
#include "graphics/graphics.h"


namespace kl {
    struct Texture
    {
        Image image;
        dx::Texture texture;

        dx::TargetView target_view;
        dx::DepthView depth_view;
        dx::ShaderView shader_view;
        dx::AccessView access_view;

        Texture(GPU& gpu);

        void reload(bool has_unordered_access = false, bool is_target = false);
        bool reload_as_cube();

        void create_target_view(const dx::TargetViewDescriptor* descriptor = nullptr);
        void create_depth_view(const dx::DepthViewDescriptor* descriptor = nullptr);
        void create_shader_view(const dx::ShaderViewDescriptor* descriptor = nullptr);
        void create_access_view(const dx::AccessViewDescriptor* descriptor = nullptr);

        bool is_cube() const;
        Int2 resolution() const;

    private:
        GPU& m_gpu;
    };
}

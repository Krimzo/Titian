#pragma once

#include "media/media.h"
#include "graphics/graphics.h"


namespace kl
{
struct Texture
{
    Image image;
    dx::Texture texture;

    dx::TargetView target_view;
    dx::DepthView depth_view;
    dx::ShaderView shader_view;
    dx::AccessView access_view;

    Texture( GPU const& gpu );

    void reload( bool has_unordered_access = false, bool is_target = false );
    bool reload_as_cube();

    void create_target_view( dx::TargetViewDescriptor const* descriptor = nullptr );
    void create_depth_view( dx::DepthViewDescriptor const* descriptor = nullptr );
    void create_shader_view( dx::ShaderViewDescriptor const* descriptor = nullptr );
    void create_access_view( dx::AccessViewDescriptor const* descriptor = nullptr );

    bool is_cube() const;
    Int2 resolution() const;

private:
    GPU const& m_gpu;
};
}

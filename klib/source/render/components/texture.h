#pragma once

#include "media/media.h"
#include "graphics/graphics.h"


namespace kl {
    using texture_data = image;
}

namespace kl {
    class texture
    {
    public:
        gpu* gpu = nullptr;
        texture_data data_buffer = {};
        dx::texture graphics_buffer = nullptr;

        dx::target_view target_view = nullptr;
        dx::depth_view   depth_view = nullptr;
        dx::shader_view shader_view = nullptr;
        dx::access_view access_view = nullptr;

        texture(kl::gpu* gpu);
        texture(kl::gpu* gpu, const texture_data& data, bool is_cube = false, bool has_unordered_access = false, bool is_target = false);
        ~texture();

        void reload(bool has_unordered_access = false, bool is_target = false);
        bool reload_as_cube();

        void create_target_view(dx::target_view_descriptor* descriptor = nullptr);
        void  create_depth_view( dx::depth_view_descriptor* descriptor = nullptr);
        void create_shader_view(dx::shader_view_descriptor* descriptor = nullptr);
        void create_access_view(dx::access_view_descriptor* descriptor = nullptr);
    };
}

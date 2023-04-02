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
        texture_data data_buffer = {};
        dx::texture graphics_buffer = nullptr;

        dx::target_view target_view = nullptr;
        dx::depth_view   depth_view = nullptr;
        dx::shader_view shader_view = nullptr;
        dx::access_view access_view = nullptr;

        texture();
        texture(gpu* gpu, const texture_data& data, bool is_cube = false, bool has_unordered_access = false, bool is_target = false);
        ~texture();

        void reload(gpu* gpu, bool has_unordered_access = false, bool is_target = false);
        bool reload_cube(gpu* gpu);

        void create_target_view(gpu* gpu, dx::target_view_descriptor* descriptor = nullptr);
        void  create_depth_view(gpu* gpu,  dx::depth_view_descriptor* descriptor = nullptr);
        void create_shader_view(gpu* gpu, dx::shader_view_descriptor* descriptor = nullptr);
        void create_access_view(gpu* gpu, dx::access_view_descriptor* descriptor = nullptr);
    };
}

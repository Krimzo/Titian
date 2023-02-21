#pragma once

#include "apis/apis.h"


namespace kl {
    struct render_shaders
    {
        dx::vertex_shader vertex_shader = nullptr;
        dx::pixel_shader pixel_shader = nullptr;
        dx::layout input_layout = nullptr;

        // Creation
        render_shaders();
        ~render_shaders();

        // Get
        operator bool() const;
    };
}

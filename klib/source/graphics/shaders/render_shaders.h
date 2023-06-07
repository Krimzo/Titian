#pragma once

#include "graphics/shaders/shader_holder.h"


namespace kl {
    struct render_shaders
    {
        shader_holder<dx::vertex_shader> vertex_shader = nullptr;
        shader_holder<dx::pixel_shader> pixel_shader = nullptr;
        dx::layout input_layout = nullptr;

        operator bool() const;
    };
}

#pragma once

#include "graphics/shaders/shader_holder.h"


namespace kl {
    struct RenderShaders
    {
        ShaderHolder<dx::VertexShader> vertex_shader = nullptr;
        ShaderHolder<dx::PixelShader> pixel_shader = nullptr;
        dx::InputLayout input_layout;
        
        operator bool() const;
    };
}

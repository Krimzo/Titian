#pragma once

#include "graphics/shaders/shader_holder.h"


namespace kl {
    struct RenderShaders : private CBuffer
    {
        dx::InputLayout input_layout;
        dx::VertexShader vertex_shader;
        dx::PixelShader pixel_shader;

        RenderShaders(const GPU* gpu = nullptr);

        operator bool() const;

        template<typename T>
        void upload(const T& object, int index = 0)
        {
            CBuffer::upload(&object, sizeof(T));
            CBuffer::bind(ShaderType::VERTEX, index);
            CBuffer::bind(ShaderType::PIXEL, index);
        }
    };
}

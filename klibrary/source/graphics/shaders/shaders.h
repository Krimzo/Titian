#pragma once

#include "apis/apis.h"


namespace kl {
    struct GPU;
}

namespace kl {
    enum struct ShaderType : int32_t
    {
        VERTEX = 0,
        PIXEL,
        GEOMETRY,
        COMPUTE,
    };
}

namespace kl {
    struct CBuffer
    {
        const GPU* gpu;
        dx::Buffer cbuffer;

        CBuffer(const GPU* gpu);

        void upload(const void* data, UINT byte_size);
        void bind(ShaderType type, int index) const;
    };
}

namespace kl {
    template<typename S>
    struct ShaderHolder : private CBuffer
    {
        S shader = {};

        ShaderHolder(const GPU* gpu = nullptr)
            : CBuffer(gpu)
        {}

        operator bool() const
        {
            return gpu && shader;
        }

        template<typename T>
        void upload(const T& object, int index = 0)
        {
            CBuffer::upload(&object, sizeof(T));
            CBuffer::bind(type(), index);
        }

        static consteval ShaderType type()
        {
            if constexpr (std::is_same_v<S, dx::VertexShader>) {
                return ShaderType::VERTEX;
            }
            else if constexpr (std::is_same_v<S, dx::PixelShader>) {
                return ShaderType::PIXEL;
            }
            else if constexpr (std::is_same_v<S, dx::GeometryShader>) {
                return ShaderType::GEOMETRY;
            }
            else if constexpr (std::is_same_v<S, dx::ComputeShader>) {
                return ShaderType::COMPUTE;
            }
            else {
                static_assert(false, "Unsupported shader type");
            }
        }
    };
}

namespace kl {
    struct Shaders : private CBuffer
    {
        dx::InputLayout input_layout;
        dx::VertexShader vertex_shader;
        dx::PixelShader pixel_shader;

        Shaders(const GPU* gpu = nullptr);

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

namespace kl {
    using VertexShader = ShaderHolder<dx::VertexShader>;
    using PixelShader = ShaderHolder<dx::PixelShader>;
    using GeometryShader = ShaderHolder<dx::GeometryShader>;
    using ComputeShader = ShaderHolder<dx::ComputeShader>;
}

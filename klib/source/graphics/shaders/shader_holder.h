#pragma once

#include "apis/apis.h"


namespace kl {
    class GPU;
}

namespace kl {
    template<typename S>
    class ShaderHolder
    {
    public:
        GPU* gpu = nullptr;
        S shader = nullptr;
        dx::Buffer cbuffer = nullptr;

        // Construct
        ShaderHolder(kl::GPU* gpu);
        ShaderHolder(kl::GPU* gpu, const S& shader);

        // Get
        operator S() const;
        operator bool() const;

        // CBuffer
        template<typename T>
        void update_cbuffer(const T& object)
        {
            update_cbuffer(&object, sizeof(T));
        }

        void update_cbuffer(const void* data, UINT byte_size);
    };
}

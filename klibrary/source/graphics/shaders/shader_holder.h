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
        const GPU* gpu = nullptr;
        S shader = {};
        dx::Buffer cbuffer;

        // Construct
        ShaderHolder(const kl::GPU* gpu);
        ShaderHolder(const kl::GPU* gpu, const S& shader);

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

#pragma once

#include "apis/apis.h"


namespace kl {
    struct GPU;
}

namespace kl {
    template<typename S>
    struct ShaderHolder
    {
        const GPU* gpu = nullptr;
        S shader = {};
        dx::Buffer cbuffer;

        ShaderHolder(const kl::GPU* gpu);
        ShaderHolder(const kl::GPU* gpu, const S& shader);

        operator S() const;
        operator bool() const;

        template<typename T>
        void update_cbuffer(const T& object)
        {
            update_cbuffer(&object, sizeof(T));
        }

        void update_cbuffer(const void* data, UINT byte_size);
    };
}

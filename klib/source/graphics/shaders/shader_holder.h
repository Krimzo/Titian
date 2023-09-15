#pragma once

#include "apis/apis.h"


namespace kl {
    class gpu;
}

namespace kl {
    template<typename S>
    struct shader_holder
    {
        gpu* gpu = nullptr;
        S shader = nullptr;
        dx::buffer cbuffer = nullptr;

        // Construct
        shader_holder(kl::gpu* gpu);
        shader_holder(kl::gpu* gpu, const S& shader);

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

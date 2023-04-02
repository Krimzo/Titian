#pragma once

#include "apis/apis.h"

#define null nullptr


namespace kl {
    template <typename T>
    using ref = std::shared_ptr<T>;

    template <typename T, typename... Args>
    ref<T> make(const Args&... args)
    {
        return std::make_shared<T>(args...);
    }
}

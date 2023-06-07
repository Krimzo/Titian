#pragma once

#include "apis/apis.h"


namespace kl {
    template <typename T>
    using ref = std::shared_ptr<T>;

    template <typename T, typename... Args>
    ref<T> make(const Args&... args)
    {
        return std::make_shared<T>(args...);
    }
}

#pragma once

#include "apis/apis.h"


namespace kl {
    inline const int CPU_CORE_COUNT = (int) std::thread::hardware_concurrency();
}

namespace kl {
    template<typename T = int>
    inline constexpr void async_for(const T start_inclusive, const T end_exclusive, const auto loop_body)
    {
        const std::ranges::iota_view view{ start_inclusive, end_exclusive };
        std::for_each(std::execution::par, view.begin(), view.end(), loop_body);
    }
}

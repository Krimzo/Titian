#pragma once

#include "apis/apis.h"


namespace kl {
    inline const int CPU_CORE_COUNT = (int) std::thread::hardware_concurrency();
}

namespace kl {
    template<typename T>
    void async_for(T start_incl, T end_excl, const auto& loop_body)
    {
        const std::ranges::iota_view view{ start_incl, end_excl };
        std::for_each(std::execution::par, view.begin(), view.end(), loop_body);
    }

    template<typename T>
    void sync_for(T start_incl, T end_excl, const auto& loop_body)
    {
        const std::ranges::iota_view view{ start_incl, end_excl };
        std::for_each(std::execution::seq, view.begin(), view.end(), loop_body);
    }
}

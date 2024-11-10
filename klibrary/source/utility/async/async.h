#pragma once

#include "apis/apis.h"


namespace kl
{
inline int CPU_CORE_COUNT = (int) std::thread::hardware_concurrency();
}

namespace kl
{
template<typename T, typename F>
void async_for( T start_incl, T end_excl, F const& loop_body )
{
    std::ranges::iota_view view{ start_incl, end_excl };
    std::for_each( std::execution::par, view.begin(), view.end(), loop_body );
}

template<typename T, typename F>
void sync_for( T start_incl, T end_excl, F const& loop_body )
{
    std::ranges::iota_view view{ start_incl, end_excl };
    std::for_each( std::execution::seq, view.begin(), view.end(), loop_body );
}
}

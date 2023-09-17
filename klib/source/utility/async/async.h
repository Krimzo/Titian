#pragma once

#include "apis/apis.h"


namespace kl {
    inline const int CPU_CORE_COUNT = std::thread::hardware_concurrency();
}

namespace kl {
    void async_for(int64_t start_inclusive, int64_t end_exclusive, const std::function<void(int64_t)>& loop_body);
}

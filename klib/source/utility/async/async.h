#pragma once

#include "apis/apis.h"


namespace kl::async {
    inline const int core_count = std::thread::hardware_concurrency();

    void loop(int64_t start_inclusive, int64_t end_exclusive, const std::function<void(int64_t)>& loop_body);
}

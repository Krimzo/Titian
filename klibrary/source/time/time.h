#pragma once

#include "time/timer/timer.h"
#include "time/date/date.h"


namespace kl::time {
    uint64_t now();
    uint64_t cpu_frequency();

    float calculate(uint64_t start, uint64_t end);
    float delta();

    void wait(float seconds);
    bool sleep(float seconds);
}

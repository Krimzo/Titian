#pragma once

#include "time/timer/timer.h"
#include "time/date/date.h"


namespace kl::time {
    uint64_t cpu_frequency();
    uint64_t now();

    float elapsed(uint64_t start, uint64_t end);
    float elapsed(uint64_t from);
    float delta();

    void wait(float seconds);
    void sleep(float seconds);
}

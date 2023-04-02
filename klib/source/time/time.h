#pragma once


// Timer
#include "time/timer/timer.h"

// Date
#include "time/date/date.h"


namespace kl::time {
    uint64_t now();
    uint64_t cpu_frequency();

    float calculate(uint64_t start, uint64_t end);
    float get_interval();

    void wait(float seconds);
    bool sleep(float seconds);
}

#pragma once


// Timer
#include "time/timer/timer.h"

// Date
#include "time/date/date.h"


namespace kl::time {
    time_t now();

    size_t cpu_frequency();

    float calculate(time_t start, time_t end);

    float get_interval();

    void wait(float seconds);

    bool sleep(float seconds);
}

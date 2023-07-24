#include "time/timer/timer.h"

#include "time/time.h"


kl::timer::timer()
{
    const time_t now = time::now();
    delta_start_ = now;
    delta_end_ = now;
    elapsed_start_ = now;
}

void kl::timer::reset()
{
    update_delta();
    reset_elapsed();
}

void kl::timer::update_delta()
{
    delta_start_ = delta_end_;
    delta_end_ = time::now();
}

float kl::timer::delta() const
{
    return time::calculate(delta_start_, delta_end_);
}

void kl::timer::reset_elapsed()
{
    elapsed_start_ = time::now();
}

float kl::timer::elapsed() const
{
    return time::calculate(elapsed_start_, time::now());
}

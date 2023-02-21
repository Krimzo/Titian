#include "time/timer/timer.h"

#include "time/time.h"


kl::timer::timer()
{
    const time_t now = time::now();
    interval_start_ = now;
    interval_end_ = now;
    elapsed_start_ = now;
}

void kl::timer::reset()
{
    update_interval();
    update_elapsed();
}

void kl::timer::update_interval()
{
    interval_start_ = interval_end_;
    interval_end_ = time::now();
}

float kl::timer::get_interval() const
{
    return time::calculate(interval_start_, interval_end_);
}

void kl::timer::update_elapsed()
{
    elapsed_start_ = time::now();
}

float kl::timer::get_elapsed() const
{
    return time::calculate(elapsed_start_, time::now());
}

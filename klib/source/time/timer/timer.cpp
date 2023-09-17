#include "klib.h"


kl::Timer::Timer()
{
    const time_t now = time::now();
    m_delta_start = now;
    m_delta_end = now;
    m_elapsed_start = now;
}

void kl::Timer::reset()
{
    update_delta();
    reset_elapsed();
}

void kl::Timer::update_delta()
{
    m_delta_start = m_delta_end;
    m_delta_end = time::now();
}

float kl::Timer::delta() const
{
    return time::calculate(m_delta_start, m_delta_end);
}

void kl::Timer::reset_elapsed()
{
    m_elapsed_start = time::now();
}

float kl::Timer::elapsed() const
{
    return time::calculate(m_elapsed_start, time::now());
}

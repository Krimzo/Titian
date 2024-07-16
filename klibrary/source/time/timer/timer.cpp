#include "klibrary.h"


kl::Timer::Timer()
{
    const uint64_t now = time::now();
    m_delta_start = now;
    m_delta_end = now;
    m_elapsed_start = now;
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
    m_old_elapsed = 0.0f;
    m_elapsed_start = time::now();
}

float kl::Timer::elapsed() const
{
    if (!m_is_running) {
        return m_old_elapsed;
    }
    const float elapsed = time::calculate(m_elapsed_start, time::now());
    return m_old_elapsed + elapsed;
}

void kl::Timer::stop()
{
    update_delta();
    reset_elapsed();
    m_is_running = false;
}

void kl::Timer::restart()
{
    update_delta();
    reset_elapsed();
    m_is_running = true;
}

void kl::Timer::pause()
{
    if (!m_is_running) {
        return;
    }
    m_old_elapsed += time::calculate(m_elapsed_start, time::now());
    m_is_running = false;
}

void kl::Timer::resume()
{
    if (m_is_running) {
        return;
    }
	update_delta();
    m_elapsed_start = time::now();
    m_is_running = true;
}

bool kl::Timer::is_running() const
{
    return m_is_running;
}

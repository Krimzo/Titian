#include "klibrary.h"


kl::Timer::Timer()
{
    restart();
}

void kl::Timer::update()
{
    const uint64_t now = time::now();
    m_delta = time::elapsed(m_last, now);
    if (m_active) {
        m_elapsed += m_delta;
    }
    m_last = now;
}

void kl::Timer::stop()
{
    m_active = false;
}

void kl::Timer::start()
{
    m_active = true;
}

void kl::Timer::reset()
{
    m_last = time::now();
    m_delta = 0.0f;
    m_elapsed = 0.0f;
    m_active = false;
}

void kl::Timer::restart()
{
    reset();
    start();
}

bool kl::Timer::active() const
{
    return m_active;
}

float kl::Timer::delta() const
{
    return m_delta;
}

float kl::Timer::elapsed() const
{
	return m_elapsed;
}

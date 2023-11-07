#include "klibrary.h"


// Window private
void kl::Key::update(const bool new_state)
{
    if (!m_state && new_state) {
        call_on_press();
    }
    else if (m_state && !new_state) {
        call_on_release();
    }
    m_state = new_state;
}

void kl::Key::process() const
{
    if (m_state) {
        call_on_down();
    }
}

// Callers
void kl::Key::call_on_press() const
{
    for (auto& callback : on_press) {
        callback();
    }
}

void kl::Key::call_on_down() const
{
    for (auto& callback : on_down) {
        callback();
    }
}

void kl::Key::call_on_release() const
{
    for (auto& callback : on_release) {
        callback();
    }
}

// User access
kl::Key::operator bool() const
{
    return m_state;
}

bool kl::Key::is_down() const
{
    return m_state;
}

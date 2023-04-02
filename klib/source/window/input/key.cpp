#include "window/input/key.h"


// Window private
void kl::key::update(const bool new_state)
{
    if (!state_ && new_state) {
        call_on_press();
    }
    else if (state_ && !new_state) {
        call_on_release();
    }
    state_ = new_state;
}

void kl::key::process() const
{
    if (state_) {
        call_on_down();
    }
}

// Callers
void kl::key::call_on_press() const
{
    for (auto& callback : on_press) {
        callback();
    }
}

void kl::key::call_on_down() const
{
    for (auto& callback : on_down) {
        callback();
    }
}

void kl::key::call_on_release() const
{
    for (auto& callback : on_release) {
        callback();
    }
}

// User access
kl::key::operator bool() const
{
    return state_;
}

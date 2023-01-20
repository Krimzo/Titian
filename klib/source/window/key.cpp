#include "window/key.h"


kl::key::key(const WPARAM type)
    : type_(type)
{}

kl::key::~key()
{}

bool kl::key::state() const
{
    return state_;
}

void kl::key::update_value(const WPARAM type, const bool new_state)
{
    if (type == type_) {
        if (!state_ && new_state) {
            on_press();
        }
        else if (state_ && !new_state) {
            on_release();
        }
        state_ = new_state;
    }
}

void kl::key::process() const
{
    if (state_) {
        on_down();
    }
}

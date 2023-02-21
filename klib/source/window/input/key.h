#pragma once

#include "apis/apis.h"


namespace kl {
    class key
    {
        bool state_ = false;
        WPARAM type_ = 0;

        void call_on_press() const;
        void call_on_down() const;
        void call_on_release() const;

    public:
        std::vector<std::function<void()>> on_press = {};
        std::vector<std::function<void()>> on_down = {};
        std::vector<std::function<void()>> on_release = {};

        key(WPARAM type);
        ~key();

        key(const key&) = delete;
        key(const key&&) = delete;

        void operator=(const key&) = delete;
        void operator=(const key&&) = delete;

        operator bool() const;

        void update_state(WPARAM type, bool new_state);
        void process() const;
    };
}

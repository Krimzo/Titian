#pragma once

#include "apis/apis.h"


namespace kl {
    class key
    {
        bool state_ = false;

        void update(bool new_state);
        void process() const;

        void call_on_press() const;
        void call_on_down() const;
        void call_on_release() const;

    public:
        friend class window;
        friend class keyboard;
        friend class mouse;

        std::vector<std::function<void()>> on_press = {};
        std::vector<std::function<void()>> on_down = {};
        std::vector<std::function<void()>> on_release = {};

        operator bool() const;
    };
}

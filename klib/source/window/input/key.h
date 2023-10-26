#pragma once

#include "apis/apis.h"


namespace kl {
    class Key
    {
        bool m_state = false;

        void update(bool new_state);
        void process() const;

        void call_on_press() const;
        void call_on_down() const;
        void call_on_release() const;

    public:
        friend class Window;
        friend class Keyboard;
        friend class Mouse;

        std::vector<std::function<void()>> on_press = {};
        std::vector<std::function<void()>> on_down = {};
        std::vector<std::function<void()>> on_release = {};

        operator bool() const;

        bool is_down() const;
    };
}

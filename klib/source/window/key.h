#pragma once

#include "system_handler.h"

#include <cstdint>
#include <functional>


namespace kl {
    class key
    {
        bool state_ = false;
        WPARAM type_ = 0;

    public:
        std::function<void()> on_press = [] {};
        std::function<void()> on_down = [] {};
        std::function<void()> on_release = [] {};

        explicit key(WPARAM type);
        ~key();

        key(const key&) = delete;
        key(const key&&) = delete;

        void operator=(const key&) = delete;
        void operator=(const key&&) = delete;

        [[nodiscard]] bool state() const;

        void update_value(WPARAM type, bool new_state);

        void process() const;
    };
}

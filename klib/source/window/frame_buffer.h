#pragma once

#include "window/window.h"


namespace kl {
    class frame_buffer
    {
        std::mutex lock_ = {};
        image buffer1_ = {};
        image buffer2_ = {};
        image* front_ = {};

    public:
        explicit frame_buffer(const int2& size);
        ~frame_buffer();

        frame_buffer(const frame_buffer&) = delete;
        frame_buffer(const frame_buffer&&) = delete;

        void operator=(const frame_buffer&) = delete;
        void operator=(const frame_buffer&&) = delete;
        
        [[nodiscard]] int2 size() const;

        image* back_buffer();

        void display(const window& window);

        void swap();
    };
}

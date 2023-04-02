#pragma once

#include "window/input/key.h"
#include "math/math.h"


namespace kl {
    class mouse
    {
        HWND window_ = nullptr;
        int2 position_ = {};
        bool hidden_ = false;
        int show_counter_ = 0;
        int scroll_ = 0;

        void process() const;

    public:
        friend class window;

        key left = {};
        key middle = {};
        key right = {};

        bool is_hidden() const;
        void set_hidden(bool enabled);

        int2 position(bool client = true) const;
        void set_position(const int2& position, bool client = true) const;

        float2 get_normalized_position(bool client = true) const;

        int scroll() const;
    };
}

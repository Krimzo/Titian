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

    public:
        union
        {
            struct
            {
                key left;
                key middle;
                key right;
            };

            key keys[3];
        };

        mouse();
        ~mouse();

        mouse(const mouse&) = delete;
        mouse(const mouse&&) = delete;

        void operator=(const mouse&) = delete;
        void operator=(const mouse&&) = delete;

        void bind_to_window(HWND window);
        void process() const;

        bool is_hidden() const;
        void set_hidden(bool enabled);

        int scroll() const;
        void update_scroll(HWND access_check, int delta_scroll);

        int2 position(bool client = true) const;
        void update_position(HWND access_check, const int2& position);
        void set_position(const int2& position) const;

        float2 get_normalized_position() const;
    };
}

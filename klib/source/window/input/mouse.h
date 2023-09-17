#pragma once

#include "window/input/key.h"
#include "math/math.h"


namespace kl {
    class Mouse
    {
        HWND m_window = nullptr;
        Int2 m_position = {};
        bool m_hidden = false;
        int m_show_counter = 0;
        int m_scroll = 0;

        void process() const;

    public:
        friend class Window;

        Key left = {};
        Key middle = {};
        Key right = {};

        bool is_hidden() const;
        void set_hidden(bool enabled);

        void set_position(const Int2& position, bool client = true);
        Int2 position(bool client = true) const;

        Float2 normalized_position(bool client = true) const;
        int scroll() const;
    };
}

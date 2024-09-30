#pragma once

#include "window/input/key.h"
#include "math/math.h"


namespace kl {
    struct Mouse
    {
        union
        {
            Key keys[5] = {};
            struct
            {
                Key left;
                Key right;
                Key middle;
                Key btn4;
                Key btn5;
            };
        };

        void set_position(Int2 position);
        Int2 position() const;
        Float2 norm_position() const;
        
        int scroll() const;

        void set_hidden(bool state);
        bool is_hidden() const;

        void _reload();
        void _update(int btn_num, bool new_state);

        void _set_window(HWND window);
        void _update_scroll(int scroll);
        void _update_position(Int2 position);

    private:
        bool m_hidden = false;
        int16_t m_scroll = 0;
        Int2 m_position = {};
        HWND m_window = nullptr;
    };
}

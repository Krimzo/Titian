#pragma once

#include "window/input/key.h"
#include "math/math.h"


namespace kl {
    struct Mouse
    {
    private:
        HWND m_window = nullptr;
        Int2 m_position = {};
        int16_t m_scroll = 0;
        bool m_hidden = false;

    public:
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

        void set_position(const Int2& position);
        Int2 position() const;
        Float2 norm_position() const;
        
        int scroll() const;

        void set_hidden(bool state);
        bool is_hidden() const;

        void _reload();
        void _update(int btn_num, bool new_state);

        void _set_window(HWND window);
        void _update_scroll(int scroll);
        void _update_position(const Int2& position);
    };
}

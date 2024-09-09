#pragma once

#include "apis/apis.h"


namespace kl {
    enum struct KeyState : int8_t
    {
        UP = 0,
        PRESSED,
		DOWN,
		RELEASED,
    };
}

namespace kl {
    struct Key
    {
        Key() = default;

        bool pressed() const;
        bool released() const;
        operator bool() const;

        void _reload();
        void _update(bool state);

    private:
        KeyState m_state = KeyState::UP;
    };
}

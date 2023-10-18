#pragma once

#include "window/input/key.h"


namespace kl {
    class Keyboard
    {
        void update(WPARAM type, bool new_state);
        void process() const;

    public:
        friend class Window;

        Key q = {};
        Key w = {};
        Key e = {};
        Key r = {};
        Key t = {};
        Key z = {};
        Key u = {};
        Key i = {};
        Key o = {};
        Key p = {};
        Key a = {};
        Key s = {};
        Key d = {};
        Key f = {};
        Key g = {};
        Key h = {};
        Key j = {};
        Key k = {};
        Key l = {};
        Key y = {};
        Key x = {};
        Key c = {};
        Key v = {};
        Key b = {};
        Key n = {};
        Key m = {};

        Key num0 = {};
        Key num1 = {};
        Key num2 = {};
        Key num3 = {};
        Key num4 = {};
        Key num5 = {};
        Key num6 = {};
        Key num7 = {};
        Key num8 = {};
        Key num9 = {};

        Key period = {};
        Key comma = {};
        Key plus = {};
        Key minus = {};

        Key esc = {};
        Key tab = {};
        Key caps = {};
        Key shift = {};
        Key ctrl = {};
        Key alt = {};
        Key space = {};
        Key enter = {};
        Key insert = {};
        Key delet = {};

        Key up = {};
        Key left = {};
        Key down = {};
        Key right = {};

        Key f1 = {};
        Key f2 = {};
        Key f3 = {};
        Key f4 = {};
        Key f5 = {};
        Key f6 = {};
        Key f7 = {};
        Key f8 = {};
        Key f9 = {};
        Key f10 = {};
        Key f11 = {};
        Key f12 = {};
    };
}

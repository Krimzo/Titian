#pragma once

#include "window/input/key.h"


namespace kl {
    class keyboard
    {
        void update(WPARAM type, bool new_state);
        void process() const;

    public:
        friend class window;

        key q = {};
        key w = {};
        key e = {};
        key r = {};
        key t = {};
        key z = {};
        key u = {};
        key i = {};
        key o = {};
        key p = {};
        key a = {};
        key s = {};
        key d = {};
        key f = {};
        key g = {};
        key h = {};
        key j = {};
        key k = {};
        key l = {};
        key y = {};
        key x = {};
        key c = {};
        key v = {};
        key b = {};
        key n = {};
        key m = {};

        key num0 = {};
        key num1 = {};
        key num2 = {};
        key num3 = {};
        key num4 = {};
        key num5 = {};
        key num6 = {};
        key num7 = {};
        key num8 = {};
        key num9 = {};

        key period = {};
        key comma = {};
        key plus = {};
        key minus = {};

        key esc = {};
        key tab = {};
        key caps = {};
        key shift = {};
        key ctrl = {};
        key alt = {};
        key space = {};
        key enter = {};
        key insert = {};
        key delet = {};

        key up = {};
        key left = {};
        key down = {};
        key right = {};

        key f1 = {};
        key f2 = {};
        key f3 = {};
        key f4 = {};
        key f5 = {};
        key f6 = {};
        key f7 = {};
        key f8 = {};
        key f9 = {};
        key f10 = {};
        key f11 = {};
        key f12 = {};
    };
}

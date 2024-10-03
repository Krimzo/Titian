#pragma once

#include "window/input/key.h"


namespace kl {
    struct Keyboard
    {
        union
        {
            Key keys[80] = {};
            struct
            {
                Key q;
                Key w;
                Key e;
                Key r;
                Key t;
                Key z;
                Key u;
                Key i;
                Key o;
                Key p;
                Key a;
                Key s;
                Key d;
                Key f;
                Key g;
                Key h;
                Key j;
                Key k;
                Key l;
                Key y;
                Key x;
                Key c;
                Key v;
                Key b;
                Key n;
                Key m;
                Key zero;
                Key one;
                Key two;
                Key three;
                Key four;
                Key five;
                Key six;
                Key seven;
                Key eight;
                Key nine;
                Key period;
                Key comma;
                Key plus;
                Key minus;
                Key multiply;
				Key divide;
                Key equals;
                Key lsqrbrckt;
				Key rsqrbrckt;
                Key semicolon;
                Key apostrophe;
                Key backslash;
                Key backtick;
                Key tab;
                Key space;
                Key enter;
                Key backspace;
                Key esc;

                Key caps;
                Key shift;
                Key ctrl;
                Key alt;
                Key insert;
                Key delet;
                Key home;
				Key end;
                Key pageup;
				Key pagedown;
                Key up;
                Key left;
                Key down;
                Key right;
                Key f1;
                Key f2;
                Key f3;
                Key f4;
                Key f5;
                Key f6;
                Key f7;
                Key f8;
                Key f9;
                Key f10;
                Key f11;
                Key f12;
            };
        };

        Keyboard() = default;

        void _reload();
        void _update(WPARAM type, bool new_state);

    private:
        void _update_char(char type, bool new_state);
        void _update_virtual(WPARAM type, bool new_state);
    };
}

#pragma once

#include "apis/apis.h"


namespace kl {
    template<typename T> struct Vector3;
    template<typename T> struct Vector4;
}

namespace kl {
    struct Color
    {
        byte b = 0;
        byte g = 0;
        byte r = 0;
        byte a = 255;

        Color();
        Color(byte r, byte g, byte b, byte a = 255);

        operator Vector3<float>() const;
        operator Vector4<float>() const;

        bool operator==(const Color& other) const;
        bool operator!=(const Color& other) const;

        Color gray() const;
        Color inverted() const;
        char as_ascii() const;

        Color mix(const Color& color, float ratio) const;
        Color mix(const Color& color) const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const Color& object);
}

namespace kl::colors {
    inline const Color CONSOLE = { 205, 205, 205 };
    inline const Color   BLACK = {   0,   0,   0 };
    inline const Color   WHITE = { 255, 255, 255 };
    inline const Color    GRAY = {  50,  50,  50 };
    inline const Color     RED = { 255,   0,   0 };
    inline const Color   GREEN = {   0, 255,   0 };
    inline const Color    BLUE = {   0,   0, 255 };
    inline const Color    CYAN = {  30, 180, 170 };
    inline const Color  PURPLE = { 220,   0, 220 };
    inline const Color  YELLOW = { 220, 220,   0 };
    inline const Color  ORANGE = { 255, 140,   0 };
    inline const Color MAGENTA = { 155,   0, 155 };
    inline const Color CRIMSON = { 100,   0,   0 };
    inline const Color   WHEAT = { 245, 220, 180 };
    inline const Color     SKY = { 190, 245, 255 };
}

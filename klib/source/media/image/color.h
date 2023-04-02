#pragma once

#include "apis/apis.h"


namespace kl {
    struct float3;
    struct float4;
}

namespace kl {
    struct color
    {
        byte b = 0;
        byte g = 0;
        byte r = 0;
        byte a = 255;

        // Construct
        color();
        color(byte r, byte g, byte b, byte a = 255);

        // Get
        operator kl::float3() const;
        operator kl::float4() const;

        // Compare
        bool operator==(const color& other) const;
        bool operator!=(const color& other) const;

        // Methods
        color gray() const;
        color inverted() const;
        char as_ascii() const;

        color mix(const color& color, float ratio) const;
        color mix(const color& color) const;
    };
}

namespace kl {
    std::ostream& operator<<(std::ostream& stream, const color& object);
}

namespace kl::colors {
    inline const color console = { 204, 204, 204 };
    inline const color   black = {   0,   0,   0 };
    inline const color   white = { 255, 255, 255 };
    inline const color    gray = {  50,  50,  50 };
    inline const color     red = { 255,   0,   0 };
    inline const color   green = {   0, 255,   0 };
    inline const color    blue = {   0,   0, 255 };
    inline const color    cyan = {  30, 180, 170 };
    inline const color  purple = { 220,   0, 220 };
    inline const color  yellow = { 220, 220,   0 };
    inline const color  orange = { 255, 140,   0 };
    inline const color magenta = { 155,   0, 155 };
    inline const color crimson = { 100,   0,   0 };
    inline const color   wheat = { 245, 220, 180 };
    inline const color     sky = { 190, 245, 255 };
}

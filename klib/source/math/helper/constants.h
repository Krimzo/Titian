#pragma once


namespace kl::math {
    inline constexpr float pi = 3.14159265358979f;

    inline constexpr float to_radians = pi / 180.0f;
    inline constexpr float to_degrees = 180.0f / pi;

    inline constexpr float to_float_color = 1.0f / 255.0f;
    inline constexpr float to_byte_color = 255.0f;
}

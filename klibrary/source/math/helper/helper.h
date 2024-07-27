#pragma once

#include "apis/apis.h"


namespace kl {
    inline constexpr double PI = 3.1415926535897932385;

    inline constexpr double TO_RADIANS = PI / 180.0;
    inline constexpr double TO_DEGREES = 180.0 / PI;

    inline constexpr double TO_FLOAT_COLOR = 1.0 / 255.0;
    inline constexpr double TO_BYTE_COLOR = 255.0 / 1.0;
}

namespace kl {
    // Deg trig
    template<typename T>
    constexpr T sin_deg(T angle_in_deg)
    {
        return (T) std::sin(angle_in_deg * TO_RADIANS);
    }

    template<typename T>
    constexpr T cos_deg(T angle_in_deg)
    {
        return (T) std::cos(angle_in_deg * TO_RADIANS);
    }

    template<typename T>
    constexpr T tan_deg(T angle_in_deg)
    {
        return (T) std::tan(angle_in_deg * TO_RADIANS);
    }

    template<typename T>
    constexpr T asin_deg(T value)
    {
        return T(std::asin(value) * TO_DEGREES);
    }

    template<typename T>
    constexpr T acos_deg(T value)
    {
        return T(std::acos(value) * TO_DEGREES);
    }

    template<typename T>
    constexpr T atan_deg(T value)
    {
        return T(std::atan(value) * TO_DEGREES);
    }

    // Clamp
    template<typename T>
    constexpr T clamp(T value, T lower, T upper)
    {
        return std::clamp(value, lower, upper);
    }

    template<typename T, bool Clamp = true>
    constexpr T lerp(T value, T lower, T upper)
    {
        value = lower + (upper - lower) * value;
        if constexpr (Clamp) {
            value = clamp(value, lower, upper);
        }
        return value;
    }

    template<typename T, bool Clamp = true>
    constexpr T unlerp(T value, T lower, T upper)
    {
        value = (value - lower) / (upper - lower);
        if constexpr (Clamp) {
            value = clamp(value, T(0), T(1));
        }
        return value;
    }

    // Apply
    template<int Count, typename T>
    constexpr T apply(const auto& func, const T& object)
    {
        T result = {};
        for (int i = 0; i < Count; i++) {
            result[i] = func(object[i]);
        }
        return result;
    }

    template<int Count, typename T>
    constexpr T apply(const auto& func, const T& first, const T& second)
    {
        T result = {};
        for (int i = 0; i < Count; i++) {
            result[i] = func(first[i], second[i]);
        }
        return result;
    }

    namespace helper {
        template<typename T>
        constexpr T abs(T object)
        {
            return std::abs(object);
        }

        template<typename T>
        constexpr T min(T first, T second)
        {
            return std::min(first, second);
        }

        template<typename T>
        constexpr T max(T first, T second)
        {
            return std::max(first, second);
        }
    }
}

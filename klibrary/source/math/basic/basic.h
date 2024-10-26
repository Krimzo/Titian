#pragma once

#include "apis/apis.h"


namespace kl {
    template<typename T = float>
    consteval T pi() noexcept
    {
        return T(3.1415926535897932385);
    }

    template<typename T = float>
    consteval T to_radians() noexcept
    {
        return T(pi<double>() / 180.0);
    }

    template<typename T = float>
    consteval T to_degrees() noexcept
    {
        return T(180.0 / pi<double>());
    }

    template<typename T = float>
    consteval T to_float_rgb() noexcept
    {
        return T(1.0 / 255.0);
    }

    template<typename T = float>
    consteval T to_byte_rgb() noexcept
    {
        return T(255.0 / 1.0);
    }
}

namespace kl {
    template<typename T>
        requires std::is_arithmetic_v<T>
    constexpr T abs(T object) noexcept
    {
        return (object < T(0)) ? -object : object;
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    constexpr T min(T first, T second) noexcept
    {
        return (first <= second) ? first : second;
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    constexpr T max(T first, T second) noexcept
    {
        return (first >= second) ? first : second;
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    constexpr T clamp(T value, T lower, T upper) noexcept
    {
        if (lower > upper) {
            return value;
        }
        return min(max(value, lower), upper);
    }

    template<typename T, bool Clamp = true>
        requires std::is_floating_point_v<T>
    constexpr T lerp(T value, T lower, T upper) noexcept
    {
        value = lower + (upper - lower) * value;
        if constexpr (Clamp) {
            value = clamp(value, lower, upper);
        }
        return value;
    }

    template<typename T, bool Clamp = true>
        requires std::is_floating_point_v<T>
    constexpr T unlerp(T value, T lower, T upper) noexcept
    {
        value = (value - lower) / (upper - lower);
        if constexpr (Clamp) {
            value = clamp(value, T(0), T(1));
        }
        return value;
    }
}

namespace kl {
    template<typename T>
        requires std::is_arithmetic_v<T>
    constexpr T sq(T value) noexcept
    {
        return value * value;
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T sqrt(T value) noexcept
    {
        return (T) std::sqrt(value);
    }

	template<typename T>
		requires std::is_floating_point_v<T>
	constexpr T pow(T base, T exponent) noexcept
	{
		return (T) std::pow(base, exponent);
	}
}

namespace kl {
    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T sin(T angle_in_rad) noexcept
    {
        return (T) std::sin(angle_in_rad);
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T cos(T angle_in_rad) noexcept
    {
        return (T) std::cos(angle_in_rad);
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T tan(T angle_in_rad) noexcept
    {
        return (T) std::tan(angle_in_rad);
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T asin(T value) noexcept
    {
        return (T) std::asin(value);
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T acos(T value) noexcept
    {
        return (T) std::acos(value);
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T atan(T value) noexcept
    {
        return (T) std::atan(value);
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T sin_d(T angle_in_deg) noexcept
    {
        return sin(angle_in_deg * to_radians<T>());
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T cos_d(T angle_in_deg) noexcept
    {
        return cos(angle_in_deg * to_radians<T>());
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T tan_d(T angle_in_deg) noexcept
    {
        return tan(angle_in_deg * to_radians<T>());
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T asin_d(T value) noexcept
    {
        return asin(value) * to_degrees<T>();
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T acos_d(T value) noexcept
    {
        return acos(value) * to_degrees<T>();
    }

    template<typename T>
        requires std::is_floating_point_v<T>
    constexpr T atan_d(T value) noexcept
    {
        return atan(value) * to_degrees<T>();
    }
}

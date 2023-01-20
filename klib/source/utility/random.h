#pragma once

#include "math/math.h"

#include <string>


namespace kl::random {
    bool get_bool();

    uint8_t get_byte();
    color get_color(bool gray = false);

    int get_int(int start_inclusive, int end_exclusive);
    int get_int(int end_exclusive);

    float get_float(float start_inclusive, float end_inclusive);
    float get_float(float end_inclusive);
    float get_float();

    char get_char(bool upper = false);
    std::string get_string(int length);

    template <typename T>
    vector2<T> get_vector2(T start_inclusive, T end_inclusive)
    {
        return {
            get_float(start_inclusive, end_inclusive),
            get_float(start_inclusive, end_inclusive)
        };
    }

    template <typename T>
    vector2<T> get_vector2(T end_inclusive)
    {
        return {
            get_float(end_inclusive),
            get_float(end_inclusive)
        };
    }

    template <typename T>
    vector2<T> get_vector2()
    {
        return {get_float(), get_float()};
    }

    template <typename T>
    vector3<T> get_vector3(T start_inclusive, T end_inclusive)
    {
        return {
            get_float(start_inclusive, end_inclusive),
            get_float(start_inclusive, end_inclusive),
            get_float(start_inclusive, end_inclusive)
        };
    }

    template <typename T>
    vector3<T> get_vector3(T end_inclusive)
    {
        return {
            get_float(end_inclusive),
            get_float(end_inclusive),
            get_float(end_inclusive)
        };
    }

    template <typename T>
    vector3<T> get_vector3()
    {
        return {get_float(), get_float(), get_float()};
    }

    template <typename T>
    vector4<T> get_vector4(T start_inclusive, T end_inclusive)
    {
        return {
            get_float(start_inclusive, end_inclusive),
            get_float(start_inclusive, end_inclusive),
            get_float(start_inclusive, end_inclusive),
            get_float(start_inclusive, end_inclusive)
        };
    }

    template <typename T>
    vector4<T> get_vector4(T end_inclusive)
    {
        return {
            get_float(end_inclusive),
            get_float(end_inclusive),
            get_float(end_inclusive),
            get_float(end_inclusive)
        };
    }

    template <typename T>
    vector4<T> get_vector4()
    {
        return {get_float(), get_float(), get_float(), get_float()};
    }

    template <typename T, int W, int H>
    matrix<T, W, H> get_matrix(T start_inclusive, T end_inclusive)
    {
        matrix<T, W, H> m;
        for (auto& val : m) {
            val = get_float(start_inclusive, end_inclusive);
        }
        return m;
    }

    template <typename T, int W, int H>
    matrix<T, W, H> get_matrix(T end_inclusive)
    {
        matrix<T, W, H> m;
        for (auto& val : m) {
            val = get_float(end_inclusive);
        }
        return m;
    }

    template <typename T, int W, int H>
    matrix<T, W, H> get_matrix()
    {
        matrix<T, W, H> m;
        for (auto& val : m) {
            val = get_float();
        }
        return m;
    }
}

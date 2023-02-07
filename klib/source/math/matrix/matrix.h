#pragma once

#include "math/vector/vector4.h"
#include "utility/strings.h"

#include <array>
#include <format>
#include <iomanip>


namespace kl {
    template <typename T, int W, int H>
    struct matrix : std::array<T, W* H>
    {
        matrix()
            : std::array<T, W* H>()
        {
            if constexpr (W == H) {
                for (int i = 0; i < (W * H); i += (W + 1)) {
                    get(i) = 1;
                }
            }
        }

        T& get(int index)
        {
            return (*this)[index];
        }

        const T& get(int index) const
        {
            return (*this)[index];
        }

        T& get(const int x, const int y)
        {
            return (*this)[y * W + x];
        }

        const T& get(const int x, const int y) const
        {
            return (*this)[y * W + x];
        }

        template <typename T0>
        explicit operator matrix<T0, W, H>() const
        {
            matrix<T0, W, H> temp;
            for (int i = 0; i < (W * H); i++) {
                temp[i] = T0(get(i));
            }
            return temp;
        }

        matrix<T, W, H> operator+(const matrix<T, W, H>& obj) const
        {
            matrix<T, W, H> result;
            for (int i = 0; i < (W * H); i++) {
                result[i] = get(i) + obj[i];
            }
            return result;
        }

        void operator+=(const matrix<T, W, H>& obj)
        {
            *this = *this + obj;
        }

        matrix<T, W, H> operator-(const matrix<T, W, H>& obj) const
        {
            matrix<T, W, H> result;
            for (int i = 0; i < (W * H); i++) {
                result[i] = get(i) - obj[i];
            }
            return result;
        }

        void operator-=(const matrix<T, W, H>& obj)
        {
            *this = *this - obj;
        }

        matrix<T, W, H> operator*(const T& val) const
        {
            matrix<T, W, H> result;
            for (int i = 0; i < (W * H); i++) {
                result[i] = get(i) * val;
            }
            return result;
        }

        void operator*=(const T& val)
        {
            *this = *this * val;
        }

        template <int S>
        matrix<T, S, H> operator*(const matrix<T, S, W>& obj) const
        {
            matrix<T, S, H> result = {};
            for (int y = 0; y < H; y++) {
                for (int x = 0; x < S; x++) {
                    result.get(x, y) = {};
                    for (int i = 0; i < W; i++) {
                        result.get(x, y) += get(i, y) * obj.get(x, i);
                    }
                }
            }
            return result;
        }

        template <int S>
        void operator*=(const matrix<T, S, W>& obj)
        {
            *this = *this * obj;
        }

        vector2<T> operator*(const vector2<T>& obj) const
        {
            vector2<T> result;
            if constexpr (W == 2 && H == 2) {
                for (int y = 0; y < 2; y++) {
                    for (int i = 0; i < 2; i++) {
                        result[y] += get(i, y) * obj[i];
                    }
                }
            }
            return result;
        }

        vector3<T> operator*(const vector3<T>& obj) const
        {
            vector3<T> result;
            if constexpr (W == 3 && H == 3) {
                for (int y = 0; y < 3; y++) {
                    for (int i = 0; i < 3; i++) {
                        result[y] += get(i, y) * obj[i];
                    }
                }
            }
            return result;
        }

        vector4<T> operator*(const vector4<T>& obj) const
        {
            vector4<T> result;
            if constexpr (W == 4 && H == 4) {
                for (int y = 0; y < 4; y++) {
                    for (int i = 0; i < 4; i++) {
                        result[y] += get(i, y) * obj[i];
                    }
                }
            }
            return result;
        }

        bool operator==(const matrix<T, W, H>& obj) const
        {
            for (int i = 0; i < (W * H); i++) {
                if (get(i) != obj[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const matrix<T, W, H>& obj) const
        {
            return !(*this == obj);
        }

        matrix<T, W, H> absolute() const
        {
            matrix<T, W, H> result;
            for (int i = 0; i < (W * H); i++) {
                result[i] = std::abs(get(i));
            }
            return result;
        }

        matrix<T, W, H> negate() const
        {
            return *this * -1.0f;
        }

        matrix<T, H, W> transpose() const
        {
            matrix<T, H, W> result;
            for (int y = 0; y < H; y++) {
                for (int x = 0; x < W; x++) {
                    result.get(x, y) = get(y, x);
                }
            }
            return result;
        }

        matrix<T, W - 1, H - 1> cofactor(const int index) const
        {
            return cofactor(index % W, index / W);
        }

        matrix<T, W - 1, H - 1> cofactor(const int pos_x, const int pos_y) const
        {
            matrix<T, W - 1, H - 1> result;
            if constexpr (W == H) {
                int counter = 0;
                for (int y = 0; y < H; y++) {
                    for (int x = 0; x < W; x++) {
                        if (x != pos_x && y != pos_y) {
                            result[counter++] = get(x, y);
                        }
                    }
                }
            }
            return result;
        }

        matrix<T, W, H> cofactor() const
        {
            matrix<T, W, H> result;
            if constexpr (W == H) {
                for (int y = 0; y < H; y++) {
                    for (int x = 0; x < W; x++) {
                        result.get(x, y) = (((y + x + 2) % 2) ? -1 : 1) * cofactor(y * W + x).determinant();
                    }
                }
            }
            return result;
        }

        T determinant() const
        {
            if constexpr (W == H) {
                if constexpr (W == 2) {
                    return get(0) * get(3) - get(1) * get(2);
                }
                if constexpr (W > 2) {
                    T result = {};
                    int multi = -1;
                    for (int i = 0; i < W; i++) {
                        result += (multi *= -1) * get(i) * cofactor(i).determinant();
                    }
                    return result;
                }
            }
            return {};
        }

        matrix<T, W, H> adjoint() const
        {
            if constexpr (W == H) {
                return cofactor().transpose();
            }
            return {};
        }

        matrix<T, W, H> inverse() const
        {
            if constexpr (W == H) {
                T deter = determinant();
                if (deter) {
                    return adjoint() * T(1.0f / deter);
                }
            }
            return {};
        }
    };

    template <typename T, int W, int H>
    std::ostream& operator<<(std::ostream& stream, const matrix<T, W, H>& object)
    {
        int max_lengths[W] = {};
        std::string output_data[W * H] = {};

        for (int x = 0; x < W; x++) {
            for (int y = 0; y < H; y++) {
                if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value) {
                    output_data[y * W + x] = std::format("{:.2f}", object[y * W + x]);
                }
                else {
                    output_data[y * W + x] = kl::format(object[y * W + x]);
                }
                max_lengths[x] = std::max(max_lengths[x], int(output_data[y * W + x].size()));
            }
        }

        for (int y = 0; y < H; y++) {
            stream << ((y == 0) ? char(218) : (y == (H - 1) ? char(192) : char(179)));
            for (int x = 0; x < (W - 1); x++) {
                stream << std::setw(max_lengths[x]) << output_data[y * W + x] << " ";
            }
            stream << std::setw(max_lengths[W - 1]) << output_data[y * W + (W - 1)]
                << ((y == 0) ? char(191) : (y == (H - 1) ? char(217) : char(179)))
                << (y == (H - 1) ? "" : "\n");
        }

        return stream;
    }
}

#pragma once

#include "math/math.h"

#include <vector>


namespace kl {
    class image
    {
        std::vector<color> pixels_;
        int2 size_ = {};

    public:
        static image get_screenshot();

        image() = default;
        explicit image(const int2& size, const color& color = {});
        explicit image(const std::string& filepath);

        std::vector<color>::iterator begin();
        std::vector<color>::iterator end();

        [[nodiscard]] int width() const;

        void set_width(int width);

        [[nodiscard]] int height() const;

        void set_height(int height);

        [[nodiscard]] int2 size() const;

        void set_size(const int2& size, bool scale = false);

        [[nodiscard]] color get_pixel(const int2& coords) const;

        void set_pixel(const int2& coords, const color& color);

        color& operator[](const int2& coords);
        const color& operator[](const int2& coords) const;

        color* data();

        [[nodiscard]] const color* data() const;

        [[nodiscard]] image get_square(int2 top_left, int2 bottom_right) const;

        bool load_from_file(const std::string& filepath);

        [[nodiscard]] bool save_to_file(const std::string& filepath) const;

        [[nodiscard]] image flip(bool vertical) const;

        void fill(const color& color);

        void draw_line(const int2& from, const int2& to, const color& color);

        void draw_triangle(int2 position_a, int2 position_b, int2 position_c, const color& color, bool fill = false);

        void draw_rectangle(int2 top_left, int2 bottom_right, const color& color, bool fill = false);

        void draw_circle(const int2& center, float radius, const color& color, bool fill = false);

        void draw_circle(const int2& center, const int2& outer_position, const color& color, bool fill = false);

        void draw_image(const int2& position, const image& image, bool mix_alpha = true);

        [[nodiscard]] std::string as_ascii(const int2& frame_size) const;
    };
}

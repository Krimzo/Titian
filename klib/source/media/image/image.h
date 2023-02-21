#pragma once

#include "math/math.h"


namespace kl {
    using pixel_storage = std::vector<color>;

    class image : private pixel_storage
    {
        int2 size_ = {};

    public:
        // Construct
        image();
        image(const int2& size);
        image(const std::string& filepath);

        // Get
        operator kl::color* ();
        operator const kl::color* () const;

        color& operator[](const int2& coords);
        const color& operator[](const int2& coords) const;

        bool in_bounds(const int2& coords) const;

        // Iterate
        pixel_storage::iterator begin();
        pixel_storage::iterator end();

        // Size
        int width() const;
        void set_width(int width, bool scale = false);

        int height() const;
        void set_height(int height, bool scale = false);

        int2 size() const;
        void resize(const int2& new_size);
        void resize_scaled(const int2& new_size);

        // Alter
        void fill(const color& color);

        image flip_horizontal() const;
        image flip_vertical() const;

        image get_rectangle(int2 top_left, int2 bottom_right) const;
        std::string as_ascii(const int2& frame_size) const;

        // Draw
        void draw_line(const int2& from, const int2& to, const color& color);
        void draw_triangle(int2 position_a, int2 position_b, int2 position_c, const color& color, bool fill = false);
        void draw_rectangle(int2 top_left, int2 bottom_right, const color& color, bool fill = false);
        void draw_circle(const int2& center, float radius, const color& color, bool fill = false);
        void draw_circle(const int2& center, const int2& outer_position, const color& color, bool fill = false);
        void draw_image(const int2& top_left, const image& image, bool mix_alpha = true);

        // Files
        bool load_from_file(const std::string& filepath);
        bool save_to_file(const std::string& filepath) const;
    };
}

namespace kl {
    image take_screenshot();
}

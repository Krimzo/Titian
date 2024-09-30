#pragma once

#include "math/math.h"


namespace kl {
    enum struct ImageType : int32_t
    {
        BMP = 0,
		PNG,
		JPG,
		TXT,
    };
}

namespace kl {
    struct Image
    {
        Image();
        Image(Int2 size);
        Image(const std::string_view& filepath);

        RGB* ptr();
        const RGB* ptr() const;

        int pixel_count() const;
        uint64_t byte_size() const;

        RGB& operator[](int index);
        const RGB& operator[](int index) const;

        RGB& operator[](Int2 coords);
        const RGB& operator[](Int2 coords) const;

        bool in_bounds(Int2 coords) const;
        RGB sample(const Float2 uv) const;

        int width() const;
        void set_width(int width);

        int height() const;
        void set_height(int height);

        Int2 size() const;
        void resize(Int2 new_size);
        void resize_scaled(Int2 new_size);

        void fill(RGB color);

        Image flip_horizontal() const;
        Image flip_vertical() const;

        Image rectangle(Int2 top_left, Int2 bottom_right) const;
        std::string as_ascii(Int2 frame_size) const;

        void draw_line(Int2 from, Int2 to, RGB color);
        void draw_triangle(Int2 position_a, Int2 position_b, Int2 position_c, RGB color, bool fill = false);
        void draw_rectangle(Int2 top_left, Int2 bottom_right, RGB color, bool fill = false);
        void draw_circle(Int2 center, float radius, RGB color, bool fill = false);
        void draw_circle(Int2 center, Int2 outer_position, RGB color, bool fill = false);
        void draw_image(Int2 top_left, const Image& image, bool mix_alpha = true);

        bool load_from_memory(const void* data, uint64_t byte_size);
        bool load_from_buffer(const std::string_view& buffer);
        bool load_from_file(const std::string_view& filepath);

        bool save_to_buffer(std::string& buffer, ImageType type) const;
        bool save_to_file(const std::string_view& filepath, ImageType type) const;

    private:
        std::vector<RGB> m_pixels;
        Int2 m_size;
    };
}

namespace kl {
    Image take_screenshot();
}

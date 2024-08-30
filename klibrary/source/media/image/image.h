#pragma once

#include "math/math.h"


namespace kl {
    enum class ImageType
    {
        BMP = 0,
		PNG,
		JPG,
		TXT,
    };
}

namespace kl {
    class Image
    {
    public:
        Image();
        Image(const Int2& size);
        Image(const std::string_view& filepath);

        // Get
        Color* ptr();
        const Color* ptr() const;

        int pixel_count() const;
        uint64_t byte_size() const;

        Color& operator[](int index);
        const Color& operator[](int index) const;

        Color& operator[](const Int2& coords);
        const Color& operator[](const Int2& coords) const;

        bool in_bounds(const Int2& coords) const;
        Color sample(const Float2& uv) const;

        // Size
        int width() const;
        void set_width(int width, bool scale = false);

        int height() const;
        void set_height(int height, bool scale = false);

        Int2 size() const;
        void resize(const Int2& new_size);
        void resize_scaled(const Int2& new_size);

        // Alter
        void fill(const Color& color);

        Image flip_horizontal() const;
        Image flip_vertical() const;

        Image rectangle(Int2 top_left, Int2 bottom_right) const;
        std::string as_ascii(const Int2& frame_size) const;

        // Draw
        void draw_line(const Int2& from, const Int2& to, const Color& color);
        void draw_triangle(Int2 position_a, Int2 position_b, Int2 position_c, const Color& color, bool fill = false);
        void draw_rectangle(Int2 top_left, Int2 bottom_right, const Color& color, bool fill = false);
        void draw_circle(const Int2& center, float radius, const Color& color, bool fill = false);
        void draw_circle(const Int2& center, const Int2& outer_position, const Color& color, bool fill = false);
        void draw_image(const Int2& top_left, const Image& image, bool mix_alpha = true);

        // Decoding
        bool load_from_memory(const void* data, uint64_t byte_size);
        bool load_from_buffer(const std::string_view& buffer);
        bool load_from_file(const std::string_view& filepath);

        // Encoding
        bool save_to_buffer(std::string& buffer, ImageType type) const;
        bool save_to_file(const std::string_view& filepath, ImageType type) const;

    private:
        std::vector<Color> m_pixels;
        Int2 m_size;
    };
}

namespace kl {
    Image take_screenshot();
}

#pragma once

#include "math/math.h"


namespace kl {
    using PixelStorage = std::vector<Color>;
}

namespace kl {
    class Image : private PixelStorage
    {
        Int2 m_size = {};

    public:
        // Construct
        Image();
        Image(const Int2& size);
        Image(const std::string& filepath);

        // Get
        operator kl::Color* ();
        operator const kl::Color* () const;

        int pixel_count() const;
        uint64_t byte_size() const;

        Color& operator[](const Int2& coords);
        const Color& operator[](const Int2& coords) const;

        bool in_bounds(const Int2& coords) const;
        Color sample(const Float2& uv) const;

        // Iterate
        PixelStorage::iterator begin();
        PixelStorage::iterator end();

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
        bool load_from_memory(const byte* data, uint64_t byte_size);
        bool load_from_vector(const std::vector<byte>& buffer);
        bool load_from_file(const std::string_view& filepath);

        // Encoding
        bool save_to_vector(std::vector<byte>* buffer, const std::string_view& type) const;
        bool save_to_file(const std::string_view& filepath, const std::string_view& type) const;
    };
}

namespace kl {
    Image take_screenshot();
}

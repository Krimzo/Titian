#include "media/image/image.h"

#include "window/system_handler.h"
#include "utility/console.h"
#include "utility/strings.h"
#include "memory/file.h"

#include <sstream>
#include <fstream>


kl::image kl::image::get_screenshot()
{
    const HDC screen_dc = GetDC(nullptr);
    const HDC memory_dc = CreateCompatibleDC(screen_dc);

    const int width = GetDeviceCaps(screen_dc, HORZRES);
    const int height = GetDeviceCaps(screen_dc, VERTRES);

    HBITMAP bitmap = CreateCompatibleBitmap(screen_dc, width, height);
    const HBITMAP old_bitmap = HBITMAP(SelectObject(memory_dc, bitmap));

    BitBlt(memory_dc, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
    bitmap = HBITMAP(SelectObject(memory_dc, old_bitmap));

    image result = image(int2(width, height));
    GetBitmapBits(bitmap, LONG(width * height * sizeof(color)), result.data());

    DeleteDC(memory_dc);
    DeleteDC(screen_dc);
    DeleteObject(old_bitmap);
    DeleteObject(bitmap);

    return result;
}

kl::image::image(const int2& size, const color& color)
    : size_(size)
{
    pixels_.resize(size.x * size.y);
    if (color != kl::color()) {
        fill(color);
    }
}

kl::image::image(const std::string& filepath)
{
    load_from_file(filepath);
}

std::vector<kl::color>::iterator kl::image::begin()
{
    return pixels_.begin();
}

std::vector<kl::color>::iterator kl::image::end()
{
    return pixels_.end();
}

int kl::image::width() const
{
    return size_.x;
}

void kl::image::set_width(int width)
{
    set_size({width, size_.y});
}

int kl::image::height() const
{
    return size_.y;
}

void kl::image::set_height(int height)
{
    set_size({size_.x, height});
}

kl::int2 kl::image::size() const
{
    return size_;
}

void kl::image::set_size(const int2& size, const bool scale)
{
    if (size != size_) {
        image result = image(size);

        if (scale) {
            const float2 ratio = float2(size_) / float2(size);
            for (int2 pos; pos.y < size.y; pos.y++) {
                for (pos.x = 0; pos.x < size.x; pos.x++) {
                    result[pos] = get_pixel(int2(float2(pos) * ratio));
                }
            }
        }
        else {
            for (int2 pos; pos.y < std::min(size.y, size_.y); pos.y++) {
                for (pos.x = 0; pos.x < std::min(size.x, size_.x); pos.x++) {
                    result[pos] = (*this)[pos];
                }
            }
        }

        *this = result;
    }
}

kl::color kl::image::get_pixel(const int2& coords) const
{
    if (coords.x >= 0 && coords.x < size_.x && coords.y >= 0 && coords.y < size_.y) {
        return (*this)[coords];
    }
    return {};
}

void kl::image::set_pixel(const int2& coords, const color& color)
{
    if (coords.x >= 0 && coords.x < size_.x && coords.y >= 0 && coords.y < size_.y) {
        (*this)[coords] = color;
    }
}

kl::color& kl::image::operator[](const int2& coords)
{
    return pixels_[coords.y * size_.x + coords.x];
}

const kl::color& kl::image::operator[](const int2& coords) const
{
    return pixels_[coords.y * size_.x + coords.x];
}

kl::color* kl::image::data()
{
    return pixels_.data();
}

const kl::color* kl::image::data() const
{
    return pixels_.data();
}

kl::image kl::image::get_rectangle(int2 top_left, int2 bottom_right) const
{
    if (bottom_right.x < top_left.x) {
        std::swap(top_left.x, bottom_right.x);
    }
    if (bottom_right.y < top_left.y) {
        std::swap(top_left.y, bottom_right.y);
    }

    image temp(bottom_right - top_left);
    for (int2 position; position.y < temp.height(); position.y++) {
        for (position.x = 0; position.x < temp.width(); position.x++) {
            temp.set_pixel(position, get_pixel(position + top_left));
        }
    }
    return temp;
}

bool kl::image::load_from_file(const std::string& filepath)
{
    ULONG_PTR token = NULL;
    const Gdiplus::GdiplusStartupInput startup_input = {};
    if (warning_check(GdiplusStartup(&token, &startup_input, nullptr), "Failed to init GDIPlus")) {
        return false;
    }

    {
        Gdiplus::Bitmap loaded_bitmap(strings::to_w_string(filepath).c_str());
        if (warning_check(loaded_bitmap.GetLastStatus(), "Failed to open image file \"" + filepath + "\"")) {
            return false;
        }

        set_size({ loaded_bitmap.GetWidth(), loaded_bitmap.GetHeight() });

        Gdiplus::BitmapData bitmap_data = {};
        loaded_bitmap.LockBits(nullptr, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmap_data);
        if (warning_check(!bitmap_data.Scan0, "Failed to load image data from fiel \"" + filepath + "\"")) {
            return false;
        }

        memcpy(pixels_.data(), bitmap_data.Scan0, pixels_.size() * sizeof(color));
    }

    Gdiplus::GdiplusShutdown(token);
    return true;
}

bool kl::image::save_to_file(const std::string& filepath) const
{
    static constexpr CLSID bmp_encoder_clsid = {
        0x557cf400, 0x1a04, 0x11d3, {0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}
    };
    static constexpr CLSID jpg_encoder_clsid = {
        0x557cf401, 0x1a04, 0x11d3, {0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}
    };
    static constexpr CLSID png_encoder_clsid = {
        0x557cf406, 0x1a04, 0x11d3, {0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}
    };

    const CLSID* format_to_use;

    if (const std::string extension = files::get_extension(filepath); extension == ".bmp") {
        format_to_use = &bmp_encoder_clsid;
    }
    else if (extension == ".jpg") {
        format_to_use = &jpg_encoder_clsid;
    }
    else if (extension == ".png") {
        format_to_use = &png_encoder_clsid;
    }
    else {
        if (extension == ".txt") {
            if (std::ofstream file(filepath); file.is_open()) {
                for (int y = 0; y < size_.y; y++) {
                    for (int x = 0; x < size_.x; x++) {
                        const color pixel = pixels_[y * size_.x + x];
                        write(file, x, " ", y, " => ",
                            int(pixel.r), " ",
                            int(pixel.g), " ",
                            int(pixel.b)
                        );
                    }
                }
                file.close();
                return true;
            }
        }
        return false;
    }

    ULONG_PTR token = NULL;
    const Gdiplus::GdiplusStartupInput startup_input = {};
    if (warning_check(GdiplusStartup(&token, &startup_input, nullptr), "Failed to init GDIPlus")) {
        return false;
    }

    {
        Gdiplus::Bitmap temp_bitmap(size_.x, size_.y, PixelFormat32bppARGB);
        if (warning_check(temp_bitmap.GetLastStatus(), "Failed to create bitmap")) {
            return false;
        }

        Gdiplus::BitmapData bitmap_data = {};
        temp_bitmap.LockBits(nullptr, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmap_data);
        if (warning_check(!bitmap_data.Scan0, "Failed to lock bitmap bits")) {
            return false;
        }

        memcpy(bitmap_data.Scan0, pixels_.data(), pixels_.size() * sizeof(color));
        temp_bitmap.UnlockBits(&bitmap_data);
        temp_bitmap.Save(strings::to_w_string(filepath).c_str(), format_to_use, nullptr);
    }

    Gdiplus::GdiplusShutdown(token);
    return true;
}

void kl::image::fill(const color& color)
{
    for (auto& pixel : pixels_) {
        pixel = color;
    }
}

kl::image kl::image::flip(const bool vertical) const
{
    image temp(size());
    if (vertical) {
        for (int x = 0; x < size_.x; x++) {
            for (int y = 0; y < size_.y; y++) {
                temp.pixels_[y * size_.x + x] = pixels_[(size_.y - 1 - y) * size_.x + x];
            }
        }
    }
    else {
        for (int y = 0; y < size_.y; y++) {
            for (int x = 0; x < size_.x; x++) {
                temp.pixels_[y * size_.x + x] = pixels_[y * size_.x + size_.x - 1 - x];
            }
        }
    }
    return temp;
}

void kl::image::draw_line(const int2& from, const int2& to, const color& color)
{
    const int length = std::max(std::abs(to.x - from.x), std::abs(to.y - from.y));
    const float2 increment = { float(to.x - from.x) / length, float(to.y - from.y) / length };

    float2 draw_point = float2(from);
    for (int i = 0; i <= length; i++) {
        set_pixel(int2(draw_point), color);
        draw_point += increment;
    }
}

void kl::image::draw_triangle(int2 position_a, int2 position_b, int2 position_c, const color& color, bool fill)
{
    if (fill) {
        if (position_a.y > position_b.y) {
            std::swap(position_a, position_b);
        }
        if (position_a.y > position_c.y) {
            std::swap(position_a, position_c);
        }
        if (position_b.y > position_c.y) {
            std::swap(position_b, position_c);
        }
        for (int y = position_a.y; y < position_c.y; y++) {
            draw_line(
                { math::line_x(float2((y < position_b.y) ? position_a : position_c), float2(position_b), float(y)), float(y) },
                { math::line_x(float2(position_a), float2(position_c), float(y)), float(y) },
                color
            );
        }
    }
    else {
        draw_line(position_a, position_b, color);
        draw_line(position_b, position_c, color);
        draw_line(position_c, position_a, color);
    }
}

void kl::image::draw_rectangle(int2 top_left, int2 bottom_right, const color& color, bool fill)
{
    if (fill) {
        if (top_left.y > bottom_right.y) {
            std::swap(top_left, bottom_right);
        }
        for (int y = top_left.y; y <= bottom_right.y; y++) {
            draw_line({top_left.x, y}, {bottom_right.x, y}, color);
        }
    }
    else {
        draw_line(top_left, {top_left.x, bottom_right.y}, color);
        draw_line(top_left, {bottom_right.x, top_left.y}, color);
        draw_line(bottom_right, {top_left.x, bottom_right.y}, color);
        draw_line(bottom_right, {bottom_right.x, top_left.y}, color);
    }
}

void kl::image::draw_circle(const int2& center, const float radius, const color& color, const bool fill)
{
    const float2 f_center = float2(center);
    if (fill) {
        for (int y = int(f_center.y - radius); y <= int(f_center.y + radius); y++) {
            const int x = int(f_center.x + std::sqrt(
                radius * radius - (y - f_center.y) * (y - f_center.y)));
            draw_line({2 * center.x - x, y}, {x, y}, color);
        }
    }
    else {
        for (int i = 0; i < int(2 * radius); i++) {
            const int x1 = int(f_center.x - radius + i);
            const int y1 = int(f_center.y + std::sqrt(radius * radius - (x1 - f_center.x) * (x1 - f_center.x)));
            set_pixel({x1, y1}, color);
            set_pixel({x1, 2 * center.y - y1}, color);

            const int y2 = int(f_center.y - radius + i);
            const int x2 = int(f_center.x + std::sqrt(radius * radius - (y2 - f_center.y) * (y2 - f_center.y)));
            set_pixel({x2, y2}, color);
            set_pixel({2 * center.x - x2, y2}, color);
        }
    }
}

void kl::image::draw_circle(const int2& center, const int2& outer_position, const color& color, const bool fill)
{
    draw_circle(center, float2(outer_position - center).length(), color, fill);
}

void kl::image::draw_image(const int2& position, const image& image, const bool mix_alpha)
{
    if (mix_alpha) {
        for (int2 coords; coords.y < image.height(); coords.y++) {
            for (coords.x = 0; coords.x < image.width(); coords.x++) {
                set_pixel(position + coords, get_pixel(position + coords).mix(image.get_pixel(coords)));
            }
        }
    }
    else {
        for (int2 coords; coords.y < image.height(); coords.y++) {
            for (coords.x = 0; coords.x < image.width(); coords.x++) {
                set_pixel(position + coords, image.get_pixel(coords));
            }
        }
    }
}

std::string kl::image::as_ascii(const int2& frame_size) const
{
    std::stringstream frame = {};
    const int2 increment = size_ / frame_size;
    for (int2 position = {}; position.y < frame_size.y; position.y++) {
        for (position.x = 0; position.x < frame_size.x; position.x++) {
            frame << get_pixel(position * increment).as_ascii();
        }
    }
    return frame.str();
}

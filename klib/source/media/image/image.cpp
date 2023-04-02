#include "media/image/image.h"

#include "utility/utility.h"
#include "memory/memory.h"


// Init
namespace kl_ignore {
    static const int dont_care = []()
    {
        ULONG_PTR token = NULL;
        const Gdiplus::GdiplusStartupInput startup_input = {};
        kl::error_check(GdiplusStartup(&token, &startup_input, nullptr), "Failed to init GDIPlus");
        return 0;
    }();
}

// Construct
kl::image::image()
{}

kl::image::image(const int2& size)
    : size_(size)
{
    pixel_storage::resize((size_t) size.x * size.y);
}

kl::image::image(const std::string& filepath)
{
    load_from_file(filepath);
}

// Get
kl::image::operator kl::color* ()
{
    return pixel_storage::data();
}

kl::image::operator const kl::color* () const
{
    return pixel_storage::data();
}

kl::color& kl::image::operator[](const int2& coords)
{
    return pixel_storage::at((size_t) coords.y * size_.x + coords.x);
}

const kl::color& kl::image::operator[](const int2& coords) const
{
    return pixel_storage::at((size_t) coords.y * size_.x + coords.x);
}

bool kl::image::in_bounds(const int2& coords) const
{
    return (coords.x >= 0 && coords.y >= 0 && coords.x < size_.x && coords.y < size_.y);
}

kl::color kl::image::sample(const float2& uv) const
{
    const int2 coords = {
        (int) (uv.x * size_.x),
        (int) (uv.y * size_.y),
    };
    if (in_bounds(coords)) {
        return (*this)[coords];
    }
    return {};
}

// Iterate
kl::pixel_storage::iterator kl::image::begin()
{
    return pixel_storage::begin();
}

kl::pixel_storage::iterator kl::image::end()
{
    return pixel_storage::end();
}

// Size
int kl::image::width() const
{
    return size_.x;
}

void kl::image::set_width(const int width, const bool scale)
{
    if (scale) {
        resize_scaled({ width, size_.y });
    }
    else {
        resize({ width, size_.y });
    }
}

int kl::image::height() const
{
    return size_.y;
}

void kl::image::set_height(const int height, const bool scale)
{
    if (scale) {
        resize_scaled({ size_.x, height });
    }
    else {
        resize({ size_.x, height });
    }
}

kl::int2 kl::image::size() const
{
    return size_;
}

void kl::image::resize(const int2& new_size)
{
    if (new_size == size_) { return; }

    const int min_x = min(new_size.x, size_.x);
    const int min_y = min(new_size.y, size_.y);

    image result = image(new_size);
    for (int2 position; position.y < min_y; position.y++) {
        for (position.x = 0; position.x < min_x; position.x++) {
            result[position] = (*this)[position];
        }
    }
    *this = result;
}

void kl::image::resize_scaled(const int2& new_size)
{
    if (new_size == size_) { return; }

    const float ratio_x = (float) size_.x / new_size.x;
    const float ratio_y = (float) size_.y / new_size.y;

    image result = { new_size };
    for (int2 position; position.y < new_size.y; position.y++) {
        for (position.x = 0; position.x < new_size.x; position.x++) {
            const int2 read_position = { (int) (position.x * ratio_x), (int) (position.y * ratio_y) };
            if (in_bounds(read_position)) {
                result[position] = (*this)[read_position];
            }
        }
    }
    *this = result;
}

// Alter
void kl::image::fill(const color& color)
{
    for (auto& pixel : (pixel_storage&) *this) {
        pixel = color;
    }
}

kl::image kl::image::flip_horizontal() const
{
    image result = { size() };
    for (int y = 0; y < size_.y; y++) {
        for (int x = 0; x < size_.x; x++) {
            result[{ x, y }] = (*this)[{ (size_.x - 1 - x), y }];
        }
    }
    return result;
}

kl::image kl::image::flip_vertical() const
{
    image result = { size() };
    for (int x = 0; x < size_.x; x++) {
        for (int y = 0; y < size_.y; y++) {
            result[{ x, y }] = (*this)[{ x, (size_.y - 1 - y) }];
        }
    }
    return result;
}

kl::image kl::image::get_rectangle(int2 top_left, int2 bottom_right) const
{
    if (bottom_right.x < top_left.x) {
        std::swap(top_left.x, bottom_right.x);
    }
    if (bottom_right.y < top_left.y) {
        std::swap(top_left.y, bottom_right.y);
    }

    image result = { int2(bottom_right.x - top_left.x, bottom_right.y - top_left.y) };
    for (int2 position; position.y < result.height(); position.y++) {
        for (position.x = 0; position.x < result.width(); position.x++) {
            const int2 read_position = top_left + position;
            if (in_bounds(read_position)) {
                result[position] = (*this)[read_position];
            }
        }
    }
    return result;
}

std::string kl::image::as_ascii(const int2& frame_size) const
{
    const int2 increment = size_ / frame_size;

    std::stringstream frame = {};
    for (int2 position = {}; position.y < frame_size.y; position.y++) {
        for (position.x = 0; position.x < frame_size.x; position.x++) {
            const int2 read_position = { position.x * increment.x, position.y * increment.y };
            if (in_bounds(read_position)) {
                frame << (*this)[read_position].as_ascii();
            }
        }
    }
    return frame.str();
}

// Draw
void kl::image::draw_line(const int2& from, const int2& to, const color& color)
{
    const int length = max(::abs(to.x - from.x), ::abs(to.y - from.y));
    const float2 increment = { (to.x - from.x) / (float) length, (to.y - from.y) / (float) length };

    float2 draw_point = from;
    for (int i = 0; i <= length; i++) {
        if (in_bounds(draw_point)) {
            (*this)[draw_point] = color;
        }
        draw_point += increment;
    }
}

void kl::image::draw_triangle(int2 position_a, int2 position_b, int2 position_c, const color& color, bool fill)
{
    if (!fill) {
        draw_line(position_a, position_b, color);
        draw_line(position_b, position_c, color);
        draw_line(position_c, position_a, color);
        return;
    }

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
        const float2 flt_pos_a = position_a;
        const float2 flt_pos_b = position_b;
        const float2 flt_pos_c = position_b;

        draw_line(
            { (int) line_x((y < position_b.y) ? flt_pos_a : flt_pos_c, flt_pos_b, (float) y), y },
            { (int) line_x(flt_pos_a, flt_pos_c, (float) y), y },
            color
        );
    }
}

void kl::image::draw_rectangle(int2 top_left, int2 bottom_right, const color& color, bool fill)
{
    if (!fill) {
        draw_line(top_left, { top_left.x, bottom_right.y }, color);
        draw_line(top_left, { bottom_right.x, top_left.y }, color);
        draw_line(bottom_right, { top_left.x, bottom_right.y }, color);
        draw_line(bottom_right, { bottom_right.x, top_left.y }, color);
        return;
    }

    if (top_left.y > bottom_right.y) {
        std::swap(top_left, bottom_right);
    }

    for (int y = top_left.y; y <= bottom_right.y; y++) {
        draw_line({ top_left.x, y }, { bottom_right.x, y }, color);
    }
}

void kl::image::draw_circle(const int2& center, const float radius, const color& color, const bool fill)
{
    const float2 f_center = center;

    if (fill) {
        const int start = (int) (f_center.y - radius);
        const int end = (int) (f_center.y + radius);

        for (int y = start; y <= end; y++) {
            const int x = int(f_center.x + sqrt(radius * radius - (y - f_center.y) * (y - f_center.y)));
            draw_line({ 2 * center.x - x, y }, { x, y }, color);
        }
        return;
    }

    const int end = (int) (2 * radius);

    for (int i = 0; i < end; i++) {
        // First pass
        const int x1 = (int) (f_center.x - radius + i);
        const int y1 = (int) (f_center.y + sqrt(radius * radius - (x1 - f_center.x) * (x1 - f_center.x)));

        if (const int2 write_position = { x1, y1 }; in_bounds(write_position)) {
            (*this)[write_position] = color;
        }
        if (const int2 write_position = { x1, 2 * center.y - y1 }; in_bounds(write_position)) {
            (*this)[write_position] = color;
        }

        // Second pass
        const int y2 = (int) (f_center.y - radius + i);
        const int x2 = (int) (f_center.x + sqrt(radius * radius - (y2 - f_center.y) * (y2 - f_center.y)));

        if (const int2 write_position = { x2, y2 }; in_bounds(write_position)) {
            (*this)[write_position] = color;
        }
        if (const int2 write_position = { 2 * center.x - x2, y2 }; in_bounds(write_position)) {
            (*this)[write_position] = color;
        }
    }
}

void kl::image::draw_circle(const int2& center, const int2& outer_position, const color& color, const bool fill)
{
    draw_circle(center, float2(outer_position - center).length(), color, fill);
}

void kl::image::draw_image(const int2& top_left, const image& image, const bool mix_alpha)
{
    for (int2 position; position.y < image.height(); position.y++) {
        for (position.x = 0; position.x < image.width(); position.x++) {
            const int2 write_position = { top_left.x + position.x, top_left.y + position.y };
            if (in_bounds(write_position)) {
                const color result_pixel = mix_alpha ? (*this)[write_position].mix(image[position]) : image[position];
                (*this)[write_position] = result_pixel;
            }
        }
    }
}

// Files
static constexpr CLSID bmp_encoder_clsid = {
    0x557cf400, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e }
};
static constexpr CLSID jpg_encoder_clsid = {
    0x557cf401, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e }
};
static constexpr CLSID png_encoder_clsid = {
    0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e }
};

bool kl::image::load_from_file(const std::string& filepath)
{
    Gdiplus::Bitmap loaded_bitmap(strings::to_w_string(filepath).c_str());
    if (warning_check(loaded_bitmap.GetLastStatus(), "Failed to open image file \"" + filepath + "\"")) {
        return false;
    }
    resize({ (int) loaded_bitmap.GetWidth(), (int) loaded_bitmap.GetHeight() });

    Gdiplus::BitmapData bitmap_data = {};
    loaded_bitmap.LockBits(nullptr, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmap_data);
    if (warning_check(!bitmap_data.Scan0, "Failed to load image data from fiel \"" + filepath + "\"")) {
        return false;
    }
    memcpy(pixel_storage::data(), bitmap_data.Scan0, pixel_storage::size() * sizeof(color));

    return true;
}

bool kl::image::save_to_file(const std::string& filepath) const
{
    const std::string extension = files::get_extension(filepath);

    if (extension == ".txt") {
        std::ofstream file(filepath);
        if (!file) {
            return false;
        }

        for (int y = 0; y < size_.y; y++) {
            for (int x = 0; x < size_.x; x++) {
                const color pixel = (*this)[y * size_.x + x];
                write(file, x, " ", y, " => ", (int) pixel.r, " ", (int) pixel.g, " ", (int) pixel.b);
            }
        }
        file.close();
        return true;
    }

    const CLSID* format_to_use;
    if (extension == ".bmp") {
        format_to_use = &bmp_encoder_clsid;
    }
    else if (extension == ".jpg") {
        format_to_use = &jpg_encoder_clsid;
    }
    else if (extension == ".png") {
        format_to_use = &png_encoder_clsid;
    }
    else {
        return false;
    }

    Gdiplus::Bitmap temp_bitmap(size_.x, size_.y, PixelFormat32bppARGB);
    if (warning_check(temp_bitmap.GetLastStatus(), "Failed to create bitmap")) {
        return false;
    }

    Gdiplus::BitmapData bitmap_data = {};
    temp_bitmap.LockBits(nullptr, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmap_data);
    if (warning_check(!bitmap_data.Scan0, "Failed to lock bitmap bits")) {
        return false;
    }

    memcpy(bitmap_data.Scan0, pixel_storage::data(), pixel_storage::size() * sizeof(color));
    temp_bitmap.UnlockBits(&bitmap_data);
    temp_bitmap.Save(strings::to_w_string(filepath).c_str(), format_to_use, nullptr);

    return true;
}

// Static
kl::image kl::take_screenshot()
{
    const HDC screen_dc = GetDC(nullptr);
    const HDC memory_dc = CreateCompatibleDC(screen_dc);

    const int width = GetDeviceCaps(screen_dc, HORZRES);
    const int height = GetDeviceCaps(screen_dc, VERTRES);

    HBITMAP bitmap = CreateCompatibleBitmap(screen_dc, width, height);
    const HBITMAP old_bitmap = (HBITMAP) SelectObject(memory_dc, bitmap);

    BitBlt(memory_dc, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
    bitmap = (HBITMAP) SelectObject(memory_dc, old_bitmap);

    image result = image(int2(width, height));
    GetBitmapBits(bitmap, width * height * (LONG) sizeof(color), result);

    DeleteDC(memory_dc);
    DeleteDC(screen_dc);
    DeleteObject(old_bitmap);
    DeleteObject(bitmap);

    return result;
}

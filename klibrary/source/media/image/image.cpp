#include "klibrary.h"


// Init
namespace kl_ignore {
    [[maybe_unused]] static const int DONT_CARE = []()
    {
        ULONG_PTR token = NULL;
        const Gdiplus::GdiplusStartupInput startup_input = {};
        kl::assert(!GdiplusStartup(&token, &startup_input, nullptr), "Failed to init GDIPlus");
        return 0;
    }();
}

// Construct
kl::Image::Image()
{}

kl::Image::Image(const Int2& size)
    : m_size(size)
{
    PixelStorage::resize((size_t) size.x * size.y);
}

kl::Image::Image(const std::string& filepath)
{
    load_from_file(filepath);
}

// Get
kl::Image::operator kl::Color* ()
{
    return PixelStorage::data();
}

kl::Image::operator const kl::Color* () const
{
    return PixelStorage::data();
}

int kl::Image::pixel_count() const
{
    return m_size.x * m_size.y;
}

uint64_t kl::Image::byte_size() const
{
    return pixel_count() * sizeof(Color);
}

kl::Color& kl::Image::operator[](const Int2& coords)
{
    return PixelStorage::at((size_t) coords.y * m_size.x + coords.x);
}

const kl::Color& kl::Image::operator[](const Int2& coords) const
{
    return PixelStorage::at((size_t) coords.y * m_size.x + coords.x);
}

bool kl::Image::in_bounds(const Int2& coords) const
{
    return (coords.x >= 0 && coords.y >= 0 && coords.x < m_size.x && coords.y < m_size.y);
}

kl::Color kl::Image::sample(const Float2& uv) const
{
    const Int2 coords = {
        (int) (uv.x * m_size.x),
        (int) (uv.y * m_size.y),
    };
    if (in_bounds(coords)) {
        return (*this)[coords];
    }
    return {};
}

// Iterate
kl::PixelStorage::iterator kl::Image::begin()
{
    return PixelStorage::begin();
}

kl::PixelStorage::iterator kl::Image::end()
{
    return PixelStorage::end();
}

// Size
int kl::Image::width() const
{
    return m_size.x;
}

void kl::Image::set_width(const int width, const bool scale)
{
    if (scale) {
        resize_scaled({ width, m_size.y });
    }
    else {
        resize({ width, m_size.y });
    }
}

int kl::Image::height() const
{
    return m_size.y;
}

void kl::Image::set_height(const int height, const bool scale)
{
    if (scale) {
        resize_scaled({ m_size.x, height });
    }
    else {
        resize({ m_size.x, height });
    }
}

kl::Int2 kl::Image::size() const
{
    return m_size;
}

void kl::Image::resize(const Int2& new_size)
{
    if (new_size == m_size) { return; }

    const int min_x = std::min(new_size.x, m_size.x);
    const int min_y = std::min(new_size.y, m_size.y);

    Image result { new_size };
    for (Int2 position; position.y < min_y; position.y++) {
        for (position.x = 0; position.x < min_x; position.x++) {
            result[position] = (*this)[position];
        }
    }
    *this = result;
}

void kl::Image::resize_scaled(const Int2& new_size)
{
    if (new_size == m_size) { return; }

    const float ratio_x = (float) m_size.x / new_size.x;
    const float ratio_y = (float) m_size.y / new_size.y;

    Image result = { new_size };
    for (Int2 position; position.y < new_size.y; position.y++) {
        for (position.x = 0; position.x < new_size.x; position.x++) {
            const Int2 read_position = { (int) (position.x * ratio_x), (int) (position.y * ratio_y) };
            if (in_bounds(read_position)) {
                result[position] = (*this)[read_position];
            }
        }
    }
    *this = result;
}

// Alter
void kl::Image::fill(const Color& color)
{
    for (auto& pixel : (PixelStorage&) *this) {
        pixel = color;
    }
}

kl::Image kl::Image::flip_horizontal() const
{
    Image result = { size() };
    for (int y = 0; y < m_size.y; y++) {
        for (int x = 0; x < m_size.x; x++) {
            result[{ x, y }] = (*this)[{ (m_size.x - 1 - x), y }];
        }
    }
    return result;
}

kl::Image kl::Image::flip_vertical() const
{
    Image result = { size() };
    for (int x = 0; x < m_size.x; x++) {
        for (int y = 0; y < m_size.y; y++) {
            result[{ x, y }] = (*this)[{ x, (m_size.y - 1 - y) }];
        }
    }
    return result;
}

kl::Image kl::Image::rectangle(Int2 top_left, Int2 bottom_right) const
{
    if (bottom_right.x < top_left.x) {
        std::swap(top_left.x, bottom_right.x);
    }
    if (bottom_right.y < top_left.y) {
        std::swap(top_left.y, bottom_right.y);
    }

    Image result = { Int2(bottom_right.x - top_left.x, bottom_right.y - top_left.y) };
    for (Int2 position; position.y < result.height(); position.y++) {
        for (position.x = 0; position.x < result.width(); position.x++) {
            const Int2 read_position = top_left + position;
            if (in_bounds(read_position)) {
                result[position] = (*this)[read_position];
            }
        }
    }
    return result;
}

std::string kl::Image::as_ascii(const Int2& frame_size) const
{
    const Int2 increment = m_size / frame_size;

    std::stringstream frame = {};
    for (Int2 position = {}; position.y < frame_size.y; position.y++) {
        for (position.x = 0; position.x < frame_size.x; position.x++) {
            const Int2 read_position = { position.x * increment.x, position.y * increment.y };
            if (in_bounds(read_position)) {
                frame << (*this)[read_position].as_ascii();
            }
        }
    }
    return frame.str();
}

// Draw
void kl::Image::draw_line(const Int2& from, const Int2& to, const Color& color)
{
    const int length = std::max(::abs(to.x - from.x), ::abs(to.y - from.y));
    const Float2 increment = { (to.x - from.x) / (float) length, (to.y - from.y) / (float) length };

    Float2 draw_point = from;
    for (int i = 0; i <= length; i++) {
        if (in_bounds(draw_point)) {
            (*this)[draw_point] = color;
        }
        draw_point += increment;
    }
}

void kl::Image::draw_triangle(Int2 position_a, Int2 position_b, Int2 position_c, const Color& color, bool fill)
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
        const Float2 flt_pos_a = position_a;
        const Float2 flt_pos_b = position_b;
        const Float2 flt_pos_c = position_b;

        draw_line(
            { (int) line_x((y < position_b.y) ? flt_pos_a : flt_pos_c, flt_pos_b, (float) y), y },
            { (int) line_x(flt_pos_a, flt_pos_c, (float) y), y },
            color
        );
    }
}

void kl::Image::draw_rectangle(Int2 top_left, Int2 bottom_right, const Color& color, bool fill)
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

void kl::Image::draw_circle(const Int2& center, const float radius, const Color& color, const bool fill)
{
    const Float2 f_center = center;

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

        if (const Int2 write_position = { x1, y1 }; in_bounds(write_position)) {
            (*this)[write_position] = color;
        }
        if (const Int2 write_position = { x1, 2 * center.y - y1 }; in_bounds(write_position)) {
            (*this)[write_position] = color;
        }

        // Second pass
        const int y2 = (int) (f_center.y - radius + i);
        const int x2 = (int) (f_center.x + sqrt(radius * radius - (y2 - f_center.y) * (y2 - f_center.y)));

        if (const Int2 write_position = { x2, y2 }; in_bounds(write_position)) {
            (*this)[write_position] = color;
        }
        if (const Int2 write_position = { 2 * center.x - x2, y2 }; in_bounds(write_position)) {
            (*this)[write_position] = color;
        }
    }
}

void kl::Image::draw_circle(const Int2& center, const Int2& outer_position, const Color& color, const bool fill)
{
    draw_circle(center, Float2(outer_position - center).length(), color, fill);
}

void kl::Image::draw_image(const Int2& top_left, const Image& image, const bool mix_alpha)
{
    for (Int2 position; position.y < image.height(); position.y++) {
        for (position.x = 0; position.x < image.width(); position.x++) {
            const Int2 write_position = { top_left.x + position.x, top_left.y + position.y };
            if (in_bounds(write_position)) {
                const Color result_pixel = mix_alpha ? (*this)[write_position].mix(image[position]) : image[position];
                (*this)[write_position] = result_pixel;
            }
        }
    }
}

// Formats
static constexpr CLSID bmp_encoder_clsid = {
    0x557cf400, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e }
};
static constexpr CLSID jpg_encoder_clsid = {
    0x557cf401, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e }
};
static constexpr CLSID png_encoder_clsid = {
    0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e }
};

// Decoding
bool kl::Image::load_from_memory(const byte* data, const uint64_t byte_size)
{
    const ComRef<IStream> stream{ SHCreateMemStream(data, (UINT) byte_size) };
    Gdiplus::Bitmap loaded_bitmap(stream.get());
    if (!verify(!loaded_bitmap.GetLastStatus(), "Failed to decode image")) {
        return false;
    }

    Gdiplus::BitmapData bitmap_data{};
    loaded_bitmap.LockBits(nullptr, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmap_data);
    if (!verify(bitmap_data.Scan0, "Failed to load image data")) {
        return false;
    }

    resize({ int(bitmap_data.Width), int(bitmap_data.Height) });
    memcpy(PixelStorage::data(), bitmap_data.Scan0, PixelStorage::size() * sizeof(Color));
    return true;
}

bool kl::Image::load_from_vector(const std::vector<byte>& buffer)
{
    return load_from_memory(buffer.data(), buffer.size());
}

bool kl::Image::load_from_file(const std::string_view& filepath)
{
    const auto file_data = read_file(filepath);
    return load_from_vector(file_data);
}

// Encoding
bool kl::Image::save_to_vector(std::vector<byte>* buffer, const ImageType type) const
{
    const CLSID* format_to_use;
    if (type == ImageType::BMP) {
        format_to_use = &bmp_encoder_clsid;
    }
    else if (type == ImageType::PNG) {
        format_to_use = &png_encoder_clsid;
    }
    else if (type == ImageType::JPG) {
        format_to_use = &jpg_encoder_clsid;
    }
    else {
        return false;
    }

    Gdiplus::Bitmap bitmap(m_size.x, m_size.y, PixelFormat32bppARGB);
    if (!verify(!bitmap.GetLastStatus(), "Failed to create bitmap")) {
        return false;
    }

    Gdiplus::BitmapData bitmap_data{};
    bitmap.LockBits(nullptr, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bitmap_data);
    if (!verify(bitmap_data.Scan0, "Failed to lock bitmap bits")) {
        return false;
    }

    memcpy(bitmap_data.Scan0, PixelStorage::data(), PixelStorage::size() * sizeof(Color));
    bitmap.UnlockBits(&bitmap_data);

    const ComRef<IStream> stream{ SHCreateMemStream(nullptr, 0) };
    bitmap.Save(stream.get(), format_to_use, nullptr);

    STATSTG stream_info{};
    stream->Stat(&stream_info, STATFLAG_NONAME);
    buffer->resize(stream_info.cbSize.QuadPart);

    stream->Seek({}, STREAM_SEEK_SET, nullptr);
    stream->Read(buffer->data(), (ULONG) buffer->size(), nullptr);
    return true;
}

bool kl::Image::save_to_file(const std::string_view& filepath, ImageType type) const
{
    if (type == ImageType::TXT) {
        std::ofstream file(filepath.data());
        if (!file) {
            return false;
        }

        for (int y = 0; y < m_size.y; y++) {
            for (int x = 0; x < m_size.x; x++) {
                const Color pixel = (*this)[y * m_size.x + x];
                write(file, x, " ", y, " => ", (int) pixel.r, " ", (int) pixel.g, " ", (int) pixel.b);
            }
        }
        file.close();
        return true;
    }

    std::vector<byte> buffer{};
    if (!save_to_vector(&buffer, type)) {
        return false;
    }
    return write_file(filepath, buffer);
}

// Static
kl::Image kl::take_screenshot()
{
    const HDC screen_dc = GetDC(nullptr);
    const HDC memory_dc = CreateCompatibleDC(screen_dc);

    const int width = GetDeviceCaps(screen_dc, HORZRES);
    const int height = GetDeviceCaps(screen_dc, VERTRES);

    HBITMAP bitmap = CreateCompatibleBitmap(screen_dc, width, height);
    const HBITMAP old_bitmap = (HBITMAP) SelectObject(memory_dc, bitmap);

    BitBlt(memory_dc, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
    bitmap = (HBITMAP) SelectObject(memory_dc, old_bitmap);

    Image result = { Int2{ width, height } };
    GetBitmapBits(bitmap, width * height * (LONG) sizeof(Color), result);

    DeleteDC(memory_dc);
    DeleteDC(screen_dc);
    DeleteObject(old_bitmap);
    DeleteObject(bitmap);

    return result;
}

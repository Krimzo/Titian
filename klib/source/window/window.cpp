#include "window/window.h"

#include "utility/console.h"


kl::window::window(const int2& size, const std::string& name)
    : name_(name)
{
    // Instance
    instance_ = GetModuleHandleA(nullptr);

    // Registering window class
    WNDCLASSEXA window_class = {};
    window_class.cbSize = sizeof(WNDCLASSEXA);
    window_class.style = CS_OWNDC;
    window_class.lpfnWndProc = [](const HWND window_handle, const UINT message, const WPARAM w_param, const LPARAM l_param)
    {
        return ((window*) GetWindowLongPtrA(window_handle, GWLP_USERDATA))->window_procedure(window_handle, message, w_param, l_param);
    };
    window_class.hInstance = instance_;
    window_class.lpszClassName = name.c_str();
    error_check(!RegisterClassExA(&window_class), "Failed to register window class");

    // Creating the window
    window_style_ = WS_OVERLAPPEDWINDOW;

    RECT size_buffer = { 0, 0, LONG(size.x), LONG(size.y) };
    AdjustWindowRect(&size_buffer, window_style_, false);

    const int2 new_size = { size_buffer.right - size_buffer.left, size_buffer.bottom - size_buffer.top };
    const int2 new_position = int2(screen::size / 2 - new_size / 2);

    window_ = CreateWindowExA(NULL, name.c_str(), name.c_str(), window_style_, new_position.x, new_position.y, new_size.x, new_size.y, nullptr, nullptr, instance_, nullptr);
    error_check(!window_, "Failed to create window");

    // Getting data
    device_context_ = GetDC(window_);
    window_style_ = GetWindowLongA(window_, GWL_STYLE);

    // Setting data
    SetWindowLongPtrA(window_, GWLP_USERDATA, LONG_PTR(this));
    ShowWindow(window_, SW_SHOW);
    SetCursor(LoadCursorA(nullptr, LPCSTR(IDC_ARROW)));
    mouse.bind_to_window(window_);
}

kl::window::~window()
{
    // Clearing DC
    ReleaseDC(window_, device_context_);
    DeleteDC(device_context_);

    // Clearing window
    DestroyWindow(window_);
    UnregisterClassA(name_.c_str(), instance_);
}

HWND kl::window::get_window() const
{
    return window_;
}

bool kl::window::process(const bool wait)
{
    MSG message;
    if (wait) {
        GetMessageA(&message, window_, 0, 0);
        handle_message(message);
    }
    else {
        while (PeekMessageA(&message, window_, 0, 0, PM_REMOVE)) {
            handle_message(message);
        }
    }

    keyboard.process();
    mouse.process();

    return is_open();
}

bool kl::window::is_open() const
{
    return IsWindow(window_);
}

void kl::window::close() const
{
    PostMessageA(window_, WM_CLOSE, NULL, NULL);
}

bool kl::window::is_resizeable() const
{
    if (!in_fullscreen_) {
        return resizeable_;
    }
    return false;
}

void kl::window::set_resizeable(const bool enabled)
{
    if (!in_fullscreen_) {
        if (!resizeable_ && enabled) {
            SetWindowLongA(window_, GWL_STYLE, GetWindowLongA(window_, GWL_STYLE) | WS_SIZEBOX | WS_MAXIMIZEBOX);
            window_style_ = GetWindowLongA(window_, GWL_STYLE);
        }
        else if (resizeable_ && !enabled) {
            SetWindowLongA(window_, GWL_STYLE, GetWindowLongA(window_, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
            window_style_ = GetWindowLongA(window_, GWL_STYLE);
        }
        resizeable_ = enabled;
    }
}

void kl::window::maximize() const
{
    ShowWindow(window_, SW_MAXIMIZE);
}

void kl::window::minimize() const
{
    ShowWindow(window_, SW_MINIMIZE);
}

bool kl::window::is_in_fullscreen() const
{
    return in_fullscreen_;
}

void kl::window::set_fullscreen(const bool enable)
{
    if (!in_fullscreen_ && enable) {
        GetWindowPlacement(window_, &placement_);
        SetWindowLongA(window_, GWL_STYLE, LONG(window_style_) & ~WS_OVERLAPPEDWINDOW);
        SetWindowPos(window_, HWND_TOP, 0, 0, screen::size.x, screen::size.y, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
    else if (in_fullscreen_ && !enable) {
        SetWindowLongA(window_, GWL_STYLE, LONG(window_style_));
        SetWindowPlacement(window_, &placement_);
        SetWindowPos(window_, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
    in_fullscreen_ = enable;
}

kl::int2 kl::window::position(const bool client) const
{
    RECT rect = {};
    if (client) {
        GetClientRect(window_, &rect);
    }
    else {
        GetWindowRect(window_, &rect);
    }
    return { rect.left, rect.top };
}

void kl::window::set_position(const int2& position) const
{
    if (!in_fullscreen_) {
        const int2 size = this->size(false);
        MoveWindow(window_, position.x, position.y, int(size.x), int(size.y), false);
    }
}

int kl::window::width() const
{
    return size().x;
}

void kl::window::set_width(int width) const
{
    set_size({ width, height() });
}

int kl::window::height() const
{
    return size().y;
}

void kl::window::set_height(int height) const
{
    set_size({ width(), height });
}

kl::int2 kl::window::size(const bool client) const
{
    RECT rect = {};
    if (client) {
        GetClientRect(window_, &rect);
    }
    else {
        GetWindowRect(window_, &rect);
    }
    return { rect.right - rect.left, rect.bottom - rect.top };
}

void kl::window::set_size(const int2& size, const bool client) const
{
    if (!in_fullscreen_) {
        const int2 position = this->position();
        int2 new_size = size;

        if (client) {
            RECT rect = { LONG(position.x), LONG(position.y), LONG(position.x + size.x), LONG(position.y + size.y) };
            AdjustWindowRect(&rect, window_style_, false);
            new_size = { rect.right - rect.left, rect.bottom - rect.top };
        }

        MoveWindow(window_, position.x, position.y, new_size.x, new_size.y, false);
    }
}

float kl::window::get_aspect_ratio() const
{
    const int2 win_size = size();
    return float(win_size.x) / win_size.y;
}

kl::int2 kl::window::get_frame_center() const
{
    return size() / 2;
}

void kl::window::set_title(const std::string& data) const
{
    SetWindowTextA(window_, data.c_str());
}

bool kl::window::set_icon(const std::string& filepath) const
{
    HICON loaded_icon = ExtractIconA(nullptr, filepath.c_str(), NULL);
    if (!loaded_icon) {
        return false;
    }
    SendMessageA(window_, WM_SETICON, ICON_BIG, LPARAM(loaded_icon));
    SendMessageA(window_, WM_SETICON, ICON_SMALL, LPARAM(loaded_icon));
    return true;
}

void kl::window::draw_pixel_data(const color* data, const int2& size, const int2& position) const
{
    BITMAPINFO bitmap_info = {};
    bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info.bmiHeader.biCompression = BI_RGB;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biWidth = LONG(size.x);
    bitmap_info.bmiHeader.biHeight = -int(size.y);

    StretchDIBits(device_context_, position.x, position.y, size.x, size.y, 0, 0, size.x, size.y, data, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);
}

void kl::window::draw_image(const image& image, const int2& position) const
{
    draw_pixel_data(image.data(), image.size(), position);
}

void kl::window::notify() const
{
    PostMessageA(window_, WM_NULL, NULL, NULL);
    PostMessageA(window_, WM_NULL, NULL, NULL);
}

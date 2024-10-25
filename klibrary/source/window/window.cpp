#include "klibrary.h"


kl::Window::Window(const std::string_view& name)
    : m_name(name)
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    m_instance = GetModuleHandleA(nullptr);

    WNDCLASSEXA window_class = {};
    window_class.cbSize = sizeof(WNDCLASSEXA);
    window_class.style = CS_OWNDC;
    window_class.lpfnWndProc = [](const HWND window_handle, const UINT message, const WPARAM w_param, const LPARAM l_param)
    {
        const Window* self = reinterpret_cast<const Window*>(GetWindowLongPtrA(window_handle, GWLP_USERDATA));
        return self->window_procedure(window_handle, message, w_param, l_param);
    };
    window_class.hInstance = m_instance;
    window_class.lpszClassName = name.data();
    assert(RegisterClassExA(&window_class), "Failed to register window class");

    m_window_style = WS_OVERLAPPEDWINDOW;
    RECT size_buffer = { 0, 0, 1600, 900 };
    AdjustWindowRect(&size_buffer, m_window_style, false);
    const Int2 new_size = {
        size_buffer.right - size_buffer.left,
        size_buffer.bottom - size_buffer.top,
    };
    const Int2 new_position = {
        screen::SIZE.x / 2 - new_size.x / 2,
        screen::SIZE.y / 2 - new_size.y / 2,
    };

    m_window = CreateWindowExA(NULL, name.data(), name.data(), m_window_style, new_position.x, new_position.y, new_size.x, new_size.y, nullptr, nullptr, m_instance, nullptr);
    assert(m_window, "Failed to create window");

    m_device_context = GetDC(m_window);
    m_window_style = GetWindowLongA(m_window, GWL_STYLE);

    SetWindowLongPtrA(m_window, GWLP_USERDATA, (LONG_PTR) this);
    ShowWindow(m_window, SW_SHOW);
    SetCursor(LoadCursorA(nullptr, (LPCSTR) IDC_ARROW));
    mouse._set_window(m_window);
}

kl::Window::~Window()
{
    ReleaseDC(m_window, m_device_context);
    DeleteDC(m_device_context);
    DestroyWindow(m_window);
    UnregisterClassA(m_name.data(), m_instance);
	m_window = nullptr;
}

kl::Window::operator HWND() const
{
    return m_window;
}

bool kl::Window::process()
{
    keyboard._reload();
    mouse._reload();
    MSG message{};
    while (PeekMessageA(&message, m_window, 0, 0, PM_REMOVE)) {
        handle_message(message);
    }
    return is_open();
}

bool kl::Window::is_open() const
{
    return IsWindow(m_window);
}

void kl::Window::close() const
{
    PostMessageA(m_window, WM_CLOSE, NULL, NULL);
}

bool kl::Window::is_resizeable() const
{
    if (!m_in_fullscreen) {
        return m_resizeable;
    }
    return false;
}

void kl::Window::set_resizeable(const bool enabled)
{
    if (m_in_fullscreen)
        return;

    if (!m_resizeable && enabled) {
        SetWindowLongA(m_window, GWL_STYLE, GetWindowLongA(m_window, GWL_STYLE) | WS_SIZEBOX | WS_MAXIMIZEBOX);
        m_window_style = GetWindowLongA(m_window, GWL_STYLE);
    }
    else if (m_resizeable && !enabled) {
        SetWindowLongA(m_window, GWL_STYLE, GetWindowLongA(m_window, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
        m_window_style = GetWindowLongA(m_window, GWL_STYLE);
    }
    m_resizeable = enabled;
}

int kl::Window::style() const
{
    return GetWindowLongA(m_window, GWL_STYLE);
}

void kl::Window::add_style(const int style)
{
    const int current_style = GetWindowLongA(m_window, GWL_STYLE);
    SetWindowLongA(m_window, GWL_STYLE, current_style | style);
}

void kl::Window::remove_style(const int style)
{
    const int current_style = GetWindowLongA(m_window, GWL_STYLE);
    SetWindowLongA(m_window, GWL_STYLE, current_style & ~style);
}

void kl::Window::maximize() const
{
    ShowWindow(m_window, SW_MAXIMIZE);
}

void kl::Window::minimize() const
{
    ShowWindow(m_window, SW_MINIMIZE);
}

void kl::Window::restore() const
{
    ShowWindow(m_window, SW_RESTORE);
}

bool kl::Window::is_maximized() const
{
    WINDOWPLACEMENT placement{};
    placement.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(m_window, &placement);
    return placement.showCmd == SW_MAXIMIZE;
}

bool kl::Window::is_minimized() const
{
    WINDOWPLACEMENT placement{};
    placement.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(m_window, &placement);
    return placement.showCmd == SW_MINIMIZE;
}

bool kl::Window::is_restored() const
{
    WINDOWPLACEMENT placement{};
    placement.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(m_window, &placement);
    return placement.showCmd == SW_RESTORE;
}

bool kl::Window::is_focused() const
{
    return GetForegroundWindow() == m_window;
}

bool kl::Window::in_fullscreen() const
{
    return m_in_fullscreen;
}

void kl::Window::set_fullscreen(const bool enabled)
{
    if (!m_in_fullscreen) {
        m_window_style = GetWindowLongA(m_window, GWL_STYLE);
        m_window_ex_style = GetWindowLongA(m_window, GWL_EXSTYLE);
    }
    if (m_in_fullscreen = enabled) {
        SetWindowLongA(m_window, GWL_STYLE, m_window_style & ~(WS_CAPTION | WS_THICKFRAME));
        SetWindowLongA(m_window, GWL_EXSTYLE, m_window_ex_style & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
    }
    else {
        SetWindowLongA(m_window, GWL_STYLE, m_window_style);
        SetWindowLongA(m_window, GWL_EXSTYLE, m_window_ex_style);
    }
}

kl::Int2 kl::Window::position() const
{
    RECT rect{};
    GetWindowRect(m_window, &rect);
    return { rect.left, rect.top };
}

void kl::Window::set_position(const Int2 position) const
{
    if (m_in_fullscreen)
        return;

    RECT rect{};
    GetWindowRect(m_window, &rect);
    MoveWindow(m_window, position.x, position.y,
        rect.right - rect.left, rect.bottom - rect.top, false);
}

int kl::Window::width() const
{
    return size().x;
}

void kl::Window::set_width(int width) const
{
    resize({ width, height() });
}

int kl::Window::height() const
{
    return size().y;
}

void kl::Window::set_height(int height) const
{
    resize({ width(), height });
}

kl::Int2 kl::Window::size() const
{
    RECT rect{};
    GetClientRect(m_window, &rect);
    return { rect.right - rect.left, rect.bottom - rect.top };
}

void kl::Window::resize(const Int2 size) const
{
    if (m_in_fullscreen)
        return;

    const Int2 pos = position();
    RECT rect{ pos.x, pos.y, pos.x + size.x, pos.y + size.y };
    AdjustWindowRect(&rect, m_window_style, false);
    MoveWindow(m_window, pos.x, pos.y, 
        rect.right - rect.left, rect.bottom - rect.top, false);
}

float kl::Window::aspect_ratio() const
{
    const Int2 win_size = size();
    return (float) win_size.x / (float) win_size.y;
}

kl::Int2 kl::Window::frame_center() const
{
    return (size() / 2);
}

float kl::Window::dpi() const
{
    return (float) GetDpiForWindow(m_window);
}

float kl::Window::pixels_to_dips(const float value) const
{
    return (value * 96.0f) / dpi();
}

float kl::Window::dips_to_pixels(const float value) const
{
    return (value * dpi()) / 96.0f;
}

void kl::Window::set_title(const std::string_view& data) const
{
    SetWindowTextA(m_window, data.data());
}

bool kl::Window::set_icon(const std::string_view& filepath) const
{
    HICON loaded_icon = ExtractIconA(nullptr, filepath.data(), NULL);
    if (!loaded_icon) {
        return false;
    }
    SendMessageA(m_window, WM_SETICON, ICON_BIG, (LPARAM) loaded_icon);
    SendMessageA(m_window, WM_SETICON, ICON_SMALL, (LPARAM) loaded_icon);
    return true;
}

void kl::Window::draw_pixel_data(const RGB* data, const Int2 size, const Int2 position) const
{
    BITMAPINFO bitmap_info{};
    bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info.bmiHeader.biCompression = BI_RGB;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biWidth = (LONG) size.x;
    bitmap_info.bmiHeader.biHeight = (LONG) -size.y;
    StretchDIBits(m_device_context,
        position.x, position.y,
        size.x, size.y,
        0, 0,
        size.x, size.y,
        data, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);
}

void kl::Window::draw_image(const Image& image, const Int2 position) const
{
    draw_pixel_data(image.ptr(), image.size(), position);
}

void kl::Window::set_dark_mode(const bool enabled) const
{
    const BOOL mode = BOOL(enabled);
    DwmSetWindowAttribute(m_window,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &mode, sizeof(BOOL));
}

void kl::Window::notify() const
{
    PostMessageA(m_window, WM_NULL, NULL, NULL);
    PostMessageA(m_window, WM_NULL, NULL, NULL);
}

#ifdef KL_USING_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT CALLBACK kl::Window::window_procedure(const HWND window_handle, const UINT message, const WPARAM w_param, const LPARAM l_param) const
{
    switch (message)
    {
    case WM_SIZE:
    {
        const Int2 size = { LOWORD(l_param), HIWORD(l_param) };
        if (size.x > 0 && size.y > 0) {
            for (auto& callback : on_resize) {
                callback(size);
            }
        }
    }
    break;

    case WM_MOVE:
    {
        const Int2 position = { LOWORD(l_param), HIWORD(l_param) };
        for (auto& callback : on_move) {
            callback(position);
        }
    }
    break;
    }
	return DefWindowProcA(window_handle, message, w_param, l_param);
}

void kl::Window::handle_message(const MSG& message)
{
#ifdef KL_USING_IMGUI
    TranslateMessage(&message);
    if (ImGui_ImplWin32_WndProcHandler(message.hwnd, message.message, message.wParam, message.lParam)) {
        return;
    }
#endif

    switch (message.message)
    {
#ifdef KL_USING_IMGUI
    case WM_CHAR:
        if (reinterpret_cast<const short&>(message.lParam) > 1) {
            imgui::GetIO().AddInputCharacter((int) message.wParam);
        }
        break;
#endif

    case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
        keyboard._update(message.wParam, true);
        break;
    case WM_KEYUP:
	case WM_SYSKEYUP:
        keyboard._update(message.wParam, false);
        break;

    case WM_LBUTTONDOWN:
        mouse._update(1, true);
        break;
    case WM_LBUTTONUP:
        mouse._update(1, false);
        break;

    case WM_RBUTTONDOWN:
        mouse._update(2, true);
        break;
    case WM_RBUTTONUP:
        mouse._update(2, false);
        break;

    case WM_MBUTTONDOWN:
        mouse._update(3, true);
        break;
    case WM_MBUTTONUP:
        mouse._update(3, false);
        break;

    case WM_XBUTTONDOWN:
        mouse._update(GET_XBUTTON_WPARAM(message.wParam) + 3, true);
		break;
    case WM_XBUTTONUP:
        mouse._update(GET_XBUTTON_WPARAM(message.wParam) + 3, false);
        break;

    case WM_MOUSEMOVE:
        mouse._update_position({ GET_X_LPARAM(message.lParam), GET_Y_LPARAM(message.lParam) });
        break;
    case WM_MOUSEWHEEL:
        mouse._update_scroll(GET_WHEEL_DELTA_WPARAM(message.wParam) / WHEEL_DELTA);
        break;
    }
    DispatchMessageA(&message);
}

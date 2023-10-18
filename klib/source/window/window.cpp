#include "klib.h"


// Class
kl::Window::Window(const std::string& name, const Int2& size)
    : m_name(name)
{
    // Instance
    m_instance = GetModuleHandleA(nullptr);

    // Registering window class
    WNDCLASSEXA window_class = {};
    window_class.cbSize = sizeof(WNDCLASSEXA);
    window_class.style = CS_OWNDC;
    window_class.lpfnWndProc = [](const HWND window_handle, const UINT message, const WPARAM w_param, const LPARAM l_param)
    {
        return ((Window*) GetWindowLongPtrA(window_handle, GWLP_USERDATA))->window_procedure(window_handle, message, w_param, l_param);
    };
    window_class.hInstance = m_instance;
    window_class.lpszClassName = name.c_str();
    assert(RegisterClassExA(&window_class), "Failed to register window class");

    // Creating the window
    m_window_style = WS_OVERLAPPEDWINDOW;

    RECT size_buffer = { 0, 0, (LONG) size.x, (LONG) size.y };
    AdjustWindowRect(&size_buffer, m_window_style, false);

    const Int2 new_size = {
        size_buffer.right - size_buffer.left,
        size_buffer.bottom - size_buffer.top,
    };
    const Int2 new_position = {
        screen::SIZE.x / 2 - new_size.x / 2,
        screen::SIZE.y / 2 - new_size.y / 2,
    };

    m_window = CreateWindowExA(NULL, name.c_str(), name.c_str(), m_window_style, new_position.x, new_position.y, new_size.x, new_size.y, nullptr, nullptr, m_instance, nullptr);
    assert(m_window, "Failed to create window");

    // Getting data
    m_device_context = GetDC(m_window);
    m_window_style = GetWindowLongA(m_window, GWL_STYLE);

    // Setting data
    SetWindowLongPtrA(m_window, GWLP_USERDATA, (LONG_PTR) this);
    ShowWindow(m_window, SW_SHOW);
    SetCursor(LoadCursorA(nullptr, (LPCSTR) IDC_ARROW));
    mouse.m_window = m_window;
}

kl::Window::~Window()
{
    // Clearing DC
    ReleaseDC(m_window, m_device_context);
    DeleteDC(m_device_context);

    // Clearing window
    DestroyWindow(m_window);
    UnregisterClassA(m_name.c_str(), m_instance);
}

kl::Window::operator HWND() const
{
    return m_window;
}

bool kl::Window::process(const bool wait)
{
    MSG message = {};
    if (wait) {
        GetMessageA(&message, m_window, 0, 0);
        handle_message(message);
    }
    else {
        while (PeekMessageA(&message, m_window, 0, 0, PM_REMOVE)) {
            handle_message(message);
        }
    }

    keyboard.process();
    mouse.process();

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
    if (m_in_fullscreen) { return; }

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

bool kl::Window::in_fullscreen() const
{
    return m_in_fullscreen;
}

void kl::Window::set_fullscreen(const bool enabled)
{
    if (!m_in_fullscreen) {
        m_window_style = GetWindowLong(m_window, GWL_STYLE);
        m_window_ex_style = GetWindowLong(m_window, GWL_EXSTYLE);
    }

    if (m_in_fullscreen = enabled) {
        SetWindowLong(m_window, GWL_STYLE, m_window_style & ~(WS_CAPTION | WS_THICKFRAME));
        SetWindowLong(m_window, GWL_EXSTYLE, m_window_ex_style & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
    }
    else {
        SetWindowLong(m_window, GWL_STYLE, m_window_style);
        SetWindowLong(m_window, GWL_EXSTYLE, m_window_ex_style);
    }
}

kl::Int2 kl::Window::position(const bool client) const
{
    RECT rect = {};
    if (client) {
        GetClientRect(m_window, &rect);
    }
    else {
        GetWindowRect(m_window, &rect);
    }
    return { rect.left, rect.top };
}

void kl::Window::set_position(const Int2& position) const
{
    if (!m_in_fullscreen) {
        const Int2 size = this->size(false);
        MoveWindow(m_window, position.x, position.y, size.x, size.y, false);
    }
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

kl::Int2 kl::Window::size(const bool client) const
{
    RECT rect = {};
    if (client) {
        GetClientRect(m_window, &rect);
    }
    else {
        GetWindowRect(m_window, &rect);
    }
    return { rect.right - rect.left, rect.bottom - rect.top };
}

void kl::Window::resize(const Int2& size, const bool client) const
{
    if (m_in_fullscreen) { return; }

    const Int2 position = this->position();
    Int2 new_size = size;

    if (client) {
        RECT rect = { (LONG) position.x, (LONG) position.y, (LONG) (position.x + size.x), (LONG) (position.y + size.y) };
        AdjustWindowRect(&rect, m_window_style, false);
        new_size = { rect.right - rect.left, rect.bottom - rect.top };
    }

    MoveWindow(m_window, position.x, position.y, new_size.x, new_size.y, false);
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

void kl::Window::set_title(const std::string& data) const
{
    SetWindowTextA(m_window, data.c_str());
}

bool kl::Window::set_icon(const std::string& filepath) const
{
    HICON loaded_icon = ExtractIconA(nullptr, filepath.c_str(), NULL);
    if (!loaded_icon) {
        return false;
    }
    SendMessageA(m_window, WM_SETICON, ICON_BIG, (LPARAM) loaded_icon);
    SendMessageA(m_window, WM_SETICON, ICON_SMALL, (LPARAM) loaded_icon);
    return true;
}

void kl::Window::draw_pixel_data(const Color* data, const Int2& size, const Int2& position) const
{
    BITMAPINFO bitmap_info = {};
    bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info.bmiHeader.biCompression = BI_RGB;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biWidth = (LONG) size.x;
    bitmap_info.bmiHeader.biHeight = (LONG) -size.y;

    StretchDIBits(m_device_context, position.x, position.y, size.x, size.y, 0, 0, size.x, size.y, data, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);
}

void kl::Window::draw_image(const Image& image, const Int2& position) const
{
    draw_pixel_data(image, image.size(), position);
}

void kl::Window::notify() const
{
    PostMessageA(m_window, WM_NULL, NULL, NULL);
    PostMessageA(m_window, WM_NULL, NULL, NULL);
}

// System
#ifdef KL_USING_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT CALLBACK kl::Window::window_procedure(const HWND window_handle, const UINT message, const WPARAM w_param, const LPARAM l_param) const
{
    // On-Resize
    if (message == WM_SIZE) {
        const Int2 new_size = { LOWORD(l_param), HIWORD(l_param) };
        for (auto& callback : on_resize) {
            callback(new_size);
        }
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

    switch (message.message) {
#ifdef KL_USING_IMGUI
    case WM_CHAR:
        if (*((short*) &message.lParam) > 1) {
            ImGui::GetIO().AddInputCharacter((int) message.wParam);
        }
        break;
#endif

        // Keyboard
    case WM_KEYDOWN:
        keyboard.update(message.wParam, true);
        break;
    case WM_KEYUP:
        keyboard.update(message.wParam, false);
        break;

        // LMB
    case WM_LBUTTONDOWN:
        mouse.left.update(true);
        break;
    case WM_LBUTTONUP:
        mouse.left.update(false);
        break;

        // MMB
    case WM_MBUTTONDOWN:
        mouse.middle.update(true);
        break;
    case WM_MBUTTONUP:
        mouse.middle.update(false);
        break;

        // RMB
    case WM_RBUTTONDOWN:
        mouse.right.update(true);
        break;
    case WM_RBUTTONUP:
        mouse.right.update(false);
        break;

        // Mouse
    case WM_MOUSEMOVE:
        mouse.m_position = { GET_X_LPARAM(message.lParam), GET_Y_LPARAM(message.lParam) };
        break;
    case WM_MOUSEWHEEL:
        mouse.m_scroll += GET_WHEEL_DELTA_WPARAM(message.wParam) / 120;
        break;

    default:
        DispatchMessageA(&message);
        break;
    }
}

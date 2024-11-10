#include "klibrary.h"


kl::Window::Window( std::string_view const& name )
    : m_name( name )
{
    SetProcessDpiAwarenessContext( DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 );
    m_instance = GetModuleHandleA( nullptr );

    WNDCLASSEXA window_class = {};
    window_class.cbSize = sizeof( WNDCLASSEXA );
    window_class.style = CS_OWNDC;
    window_class.lpfnWndProc = []( HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param )
    {
        Window const* self = reinterpret_cast<Window*>(GetWindowLongPtrA( window_handle, GWLP_USERDATA ));
        return self->window_procedure( window_handle, message, w_param, l_param );
    };
    window_class.hInstance = m_instance;
    window_class.lpszClassName = name.data();
    assert( RegisterClassExA( &window_class ), "Failed to register window class" );

    RECT size_buffer = { 0, 0, 1600, 900 };
    AdjustWindowRect( &size_buffer, WS_OVERLAPPEDWINDOW, false );
    Int2 new_size = { size_buffer.right - size_buffer.left, size_buffer.bottom - size_buffer.top };
    Int2 new_position = SCREEN_SIZE / 2 - new_size / 2;

    m_window = CreateWindowExA( NULL, name.data(), name.data(), WS_OVERLAPPEDWINDOW, new_position.x, new_position.y, new_size.x, new_size.y, nullptr, nullptr, m_instance, nullptr );
    assert( m_window, "Failed to create window" );
    m_device_context = GetDC( m_window );

    SetWindowLongPtrA( m_window, GWLP_USERDATA, (LONG_PTR) this );
    ShowWindow( m_window, SW_SHOW );
    SetCursor( LoadCursorA( nullptr, (LPCSTR) IDC_ARROW ) );
    mouse._set_window( m_window );
}

kl::Window::~Window()
{
    ReleaseDC( m_window, m_device_context );
    DeleteDC( m_device_context );
    DestroyWindow( m_window );
    UnregisterClassA( m_name.data(), m_instance );
}

HWND kl::Window::ptr() const
{
    return m_window;
}

bool kl::Window::process()
{
    keyboard._reload();
    mouse._reload();

    MSG message{};
    while ( PeekMessageA( &message, m_window, 0, 0, PM_REMOVE ) )
        handle_message( message );

    return active();
}

bool kl::Window::active() const
{
    return IsWindow( m_window );
}

void kl::Window::close() const
{
    PostMessageA( m_window, WM_CLOSE, NULL, NULL );
}

bool kl::Window::resizeable() const
{
    return style() | WS_SIZEBOX | WS_MAXIMIZEBOX;
}

void kl::Window::set_resizeable( bool enabled )
{
    if ( fullscreened() )
        return;

    if ( enabled )
    {
        add_style( WS_SIZEBOX | WS_MAXIMIZEBOX );
    }
    else
    {
        remove_style( WS_SIZEBOX | WS_MAXIMIZEBOX );
    }
}

bool kl::Window::fullscreened() const
{
    return !(style() | WS_CAPTION | WS_THICKFRAME);
}

void kl::Window::set_fullscreen( bool enabled )
{
    if ( enabled )
    {
        remove_style( WS_CAPTION | WS_THICKFRAME );
        maximize();
    }
    else
    {
        add_style( WS_CAPTION | WS_THICKFRAME );
        restore();
    }
}

LONG kl::Window::style() const
{
    return GetWindowLongA( m_window, GWL_STYLE );
}

void kl::Window::add_style( LONG style_val )
{
    SetWindowLongA( m_window, GWL_STYLE, style() | style_val );
}

void kl::Window::remove_style( LONG style_val )
{
    SetWindowLongA( m_window, GWL_STYLE, style() & ~style_val );
}

bool kl::Window::focused() const
{
    return GetForegroundWindow() == m_window;
}

void kl::Window::maximize() const
{
    ShowWindow( m_window, SW_MAXIMIZE );
}

void kl::Window::minimize() const
{
    ShowWindow( m_window, SW_MINIMIZE );
}

void kl::Window::restore() const
{
    ShowWindow( m_window, SW_RESTORE );
}

bool kl::Window::maximized() const
{
    WINDOWPLACEMENT placement{};
    placement.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( m_window, &placement );
    return placement.showCmd == SW_MAXIMIZE;
}

bool kl::Window::minimized() const
{
    WINDOWPLACEMENT placement{};
    placement.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( m_window, &placement );
    return placement.showCmd == SW_MINIMIZE;
}

bool kl::Window::restored() const
{
    WINDOWPLACEMENT placement{};
    placement.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( m_window, &placement );
    return placement.showCmd == SW_RESTORE;
}

kl::Int2 kl::Window::position() const
{
    RECT rect{};
    GetWindowRect( m_window, &rect );
    return { rect.left, rect.top };
}

void kl::Window::set_position( Int2 position ) const
{
    if ( fullscreened() )
        return;

    RECT rect{};
    GetWindowRect( m_window, &rect );
    MoveWindow( m_window, position.x, position.y,
        rect.right - rect.left, rect.bottom - rect.top, false );
}

int kl::Window::width() const
{
    return size().x;
}

void kl::Window::set_width( int width ) const
{
    resize( { width, height() } );
}

int kl::Window::height() const
{
    return size().y;
}

void kl::Window::set_height( int height ) const
{
    resize( { width(), height } );
}

kl::Int2 kl::Window::size() const
{
    RECT rect{};
    GetClientRect( m_window, &rect );
    return { rect.right - rect.left, rect.bottom - rect.top };
}

void kl::Window::resize( Int2 size ) const
{
    if ( fullscreened() )
        return;

    Int2 pos = position();
    RECT rect{ pos.x, pos.y, pos.x + size.x, pos.y + size.y };
    AdjustWindowRect( &rect, style(), false );
    MoveWindow( m_window, pos.x, pos.y,
        rect.right - rect.left, rect.bottom - rect.top, false );
}

float kl::Window::aspect_ratio() const
{
    Int2 win_size = size();
    return (float) win_size.x / (float) win_size.y;
}

kl::Int2 kl::Window::frame_center() const
{
    return size() / 2;
}

float kl::Window::dpi() const
{
    return (float) GetDpiForWindow( m_window );
}

float kl::Window::pixels_to_dips( float value ) const
{
    return (value * 96.0f) / dpi();
}

float kl::Window::dips_to_pixels( float value ) const
{
    return value * dpi() * (1.0f / 96.0f);
}

void kl::Window::set_title( std::string_view const& data ) const
{
    SetWindowTextA( m_window, data.data() );
}

bool kl::Window::set_icon( std::string_view const& filepath ) const
{
    HICON loaded_icon = ExtractIconA( nullptr, filepath.data(), NULL );
    if ( !loaded_icon )
        return false;

    SendMessageA( m_window, WM_SETICON, ICON_BIG, (LPARAM) loaded_icon );
    SendMessageA( m_window, WM_SETICON, ICON_SMALL, (LPARAM) loaded_icon );
    return true;
}

void kl::Window::draw_pixel_data( RGB const* data, Int2 size, Int2 position ) const
{
    BITMAPINFO bitmap_info{};
    bitmap_info.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    bitmap_info.bmiHeader.biCompression = BI_RGB;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biWidth = (LONG) size.x;
    bitmap_info.bmiHeader.biHeight = (LONG) -size.y;
    StretchDIBits( m_device_context,
        position.x, position.y,
        size.x, size.y,
        0, 0,
        size.x, size.y,
        data, &bitmap_info, DIB_RGB_COLORS, SRCCOPY );
}

void kl::Window::draw_image( Image const& image, Int2 position ) const
{
    draw_pixel_data( image.ptr(), image.size(), position );
}

void kl::Window::set_dark_mode( bool enabled ) const
{
    BOOL mode = BOOL( enabled );
    DwmSetWindowAttribute( m_window,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &mode, sizeof( BOOL ) );
}

void kl::Window::notify() const
{
    PostMessageA( m_window, WM_NULL, NULL, NULL );
    PostMessageA( m_window, WM_NULL, NULL, NULL );
}

#ifdef KL_USING_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
#endif

LRESULT CALLBACK kl::Window::window_procedure( HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param ) const
{
    switch ( message )
    {
    case WM_SIZE:
    {
        Int2 size = { LOWORD( l_param ), HIWORD( l_param ) };
        if ( size.x > 0 && size.y > 0 )
        {
            for ( auto& callback : on_resize )
                callback( size );
        }
    }
    break;

    case WM_MOVE:
    {
        Int2 position = { LOWORD( l_param ), HIWORD( l_param ) };
        for ( auto& callback : on_move )
            callback( position );
    }
    break;
    }
    return DefWindowProcA( window_handle, message, w_param, l_param );
}

void kl::Window::handle_message( MSG const& message )
{
#ifdef KL_USING_IMGUI
    TranslateMessage( &message );
    if ( ImGui_ImplWin32_WndProcHandler( message.hwnd, message.message, message.wParam, message.lParam ) )
    {
        return;
    }
#endif

    switch ( message.message )
    {
#ifdef KL_USING_IMGUI
    case WM_CHAR:
        if ( reinterpret_cast<short const&>(message.lParam) > 1 )
        {
            imgui::GetIO().AddInputCharacter( (int) message.wParam );
        }
        break;
#endif

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        keyboard._update( message.wParam, true );
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        keyboard._update( message.wParam, false );
        break;

    case WM_LBUTTONDOWN:
        mouse._update( 1, true );
        break;
    case WM_LBUTTONUP:
        mouse._update( 1, false );
        break;

    case WM_RBUTTONDOWN:
        mouse._update( 2, true );
        break;
    case WM_RBUTTONUP:
        mouse._update( 2, false );
        break;

    case WM_MBUTTONDOWN:
        mouse._update( 3, true );
        break;
    case WM_MBUTTONUP:
        mouse._update( 3, false );
        break;

    case WM_XBUTTONDOWN:
        mouse._update( GET_XBUTTON_WPARAM( message.wParam ) + 3, true );
        break;
    case WM_XBUTTONUP:
        mouse._update( GET_XBUTTON_WPARAM( message.wParam ) + 3, false );
        break;

    case WM_MOUSEMOVE:
        mouse._update_position( { GET_X_LPARAM( message.lParam ), GET_Y_LPARAM( message.lParam ) } );
        break;
    case WM_MOUSEWHEEL:
        mouse._update_scroll( GET_WHEEL_DELTA_WPARAM( message.wParam ) / WHEEL_DELTA );
        break;
    }
    DispatchMessageA( &message );
}

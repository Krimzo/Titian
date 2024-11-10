#include "klibrary.h"


static HANDLE _console_init = []
{
    HANDLE temp_handle = GetStdHandle( STD_OUTPUT_HANDLE );
    DWORD console_mode{};
    GetConsoleMode( temp_handle, &console_mode );
    SetConsoleMode( temp_handle, console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING );
    return temp_handle;
}();

void kl::console::set_enabled( bool state )
{
    static HWND console_window = GetConsoleWindow();
    ShowWindow( console_window, state ? SW_SHOW : SW_HIDE );
}

void kl::console::clear()
{
    CONSOLE_SCREEN_BUFFER_INFO console_screen_info{};
    GetConsoleScreenBufferInfo( _console_init, &console_screen_info );

    DWORD chars_written{};
    FillConsoleOutputCharacterA( _console_init, ' ', console_screen_info.dwSize.X * console_screen_info.dwSize.Y, {}, &chars_written );

    FillConsoleOutputAttribute(
        _console_init, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        console_screen_info.dwSize.X * console_screen_info.dwSize.Y, {}, &chars_written
    );
    move_cursor( {} );
}

void kl::console::set_cursor_enabled( bool state )
{
    CONSOLE_CURSOR_INFO cursor_info = {};
    GetConsoleCursorInfo( _console_init, &cursor_info );
    cursor_info.bVisible = state;
    SetConsoleCursorInfo( _console_init, &cursor_info );
}

void kl::console::move_cursor( Int2 position )
{
    SetConsoleCursorPosition( _console_init, { (SHORT) position.x, (SHORT) position.y } );
}

int kl::console::width()
{
    return size().x;
}

void kl::console::set_width( int width )
{
    set_size( { width, height() } );
}

int kl::console::height()
{
    return size().y;
}

void kl::console::set_height( int height )
{
    set_size( { width(), height } );
}

void kl::console::set_title( std::string_view const& text )
{
    SetConsoleTitleA( text.data() );
}

kl::Int2 kl::console::size()
{
    CONSOLE_SCREEN_BUFFER_INFO info = {};
    GetConsoleScreenBufferInfo( _console_init, &info );
    return { info.srWindow.Right - info.srWindow.Left + 1, info.srWindow.Bottom - info.srWindow.Top + 1 };
}

void kl::console::set_size( Int2 size )
{
    SMALL_RECT rect = { 0, 0, (SHORT) (size.x - 1), (SHORT) (size.y - 1) };
    SetConsoleWindowInfo( _console_init, true, &rect );
}

void kl::console::set_font( Int2 size, std::string_view const& font_name )
{
    CONSOLE_FONT_INFOEX cfi = {};
    cfi.cbSize = sizeof( CONSOLE_FONT_INFOEX );
    cfi.dwFontSize.X = (SHORT) size.x;
    cfi.dwFontSize.Y = (SHORT) size.y;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s( cfi.FaceName, convert_string( font_name ).data() );
    SetCurrentConsoleFontEx( _console_init, false, &cfi );
}

char kl::console::read()
{
    char input = 0;
    while ( _kbhit() )
        input = (char) _getch();
    return input;
}

void kl::console::wait( char to_wait_for, bool echo )
{
    if ( echo )
    {
        if ( to_wait_for > 31 && to_wait_for < 127 )
        {
            print( "Press '", to_wait_for, "' to continue..." );
        }
        else
        {
            print( "Press '", (int) to_wait_for, "' to continue..." );
        }
    }
    for ( char c = 0; (c = (char) _getch()) != to_wait_for;)
    {
        print( (int) c );
    }
}

char kl::console::wait_for_any( bool echo )
{
    if ( echo )
    {
        print( "Press any key to continue..." );
    }
    return (char) _getch();
}

void kl::console::dump( std::string_view const& data, Int2 location )
{
    DWORD ignored = 0;
    WriteConsoleOutputCharacterA( _console_init, data.data(), (DWORD) data.length(), { (SHORT) location.x, (SHORT) location.y }, &ignored );
}

void kl::console::progress_bar( std::string_view const& message, int output_y, float percentage )
{
    percentage = clamp( percentage, 0.0f, 1.0f );

    int bar_length = int( width() - message.length() - 12 );
    int finish_length = (int) float( bar_length * percentage );
    int empty_length = bar_length - finish_length;

    std::stringstream stream = {};
    stream << "  " << message << " [";
    for ( int i = 0; i < finish_length; i++ )
    {
        stream << '#';
    }
    for ( int i = 0; i < empty_length; i++ )
    {
        stream << ' ';
    }
    move_cursor( { 0, output_y } );

    printf( "%s] %3d%% ", stream.str().data(), int( percentage * 100.0f ) );
}

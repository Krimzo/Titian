#include "klibrary.h"


static const HANDLE CONSOLE_HANDLE = []
{
    const HANDLE temp_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD console_mode = {};
    GetConsoleMode(temp_handle, &console_mode);
    SetConsoleMode(temp_handle, console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    return temp_handle;
}();

void kl::console::set_enabled(const bool state)
{
    static HWND console_window = GetConsoleWindow();
    ShowWindow(console_window, state ? SW_SHOW : SW_HIDE);
}

void kl::console::clear()
{
    CONSOLE_SCREEN_BUFFER_INFO console_screen_info = {};
    GetConsoleScreenBufferInfo(CONSOLE_HANDLE, &console_screen_info);

    DWORD chars_written = {};
    FillConsoleOutputCharacterA(CONSOLE_HANDLE, ' ', console_screen_info.dwSize.X * console_screen_info.dwSize.Y, {}, &chars_written);

    FillConsoleOutputAttribute(
        CONSOLE_HANDLE, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        console_screen_info.dwSize.X * console_screen_info.dwSize.Y, {}, &chars_written
    );

    move_cursor({});
}

void kl::console::set_cursor_enabled(const bool state)
{
    CONSOLE_CURSOR_INFO cursor_info = {};
    GetConsoleCursorInfo(CONSOLE_HANDLE, &cursor_info);
    cursor_info.bVisible = state;
    SetConsoleCursorInfo(CONSOLE_HANDLE, &cursor_info);
}

void kl::console::move_cursor(const Int2& position)
{
    SetConsoleCursorPosition(CONSOLE_HANDLE, { (SHORT) position.x, (SHORT) position.y });
}

int kl::console::width()
{
    return size().x;
}

void kl::console::set_width(int width)
{
    set_size({ width, height() });
}

int kl::console::height()
{
    return size().y;
}

void kl::console::set_height(int height)
{
    set_size({ width(), height });
}

void kl::console::set_title(const std::string& text)
{
    SetConsoleTitleA(text.c_str());
}

kl::Int2 kl::console::size()
{
    CONSOLE_SCREEN_BUFFER_INFO info = {};
    GetConsoleScreenBufferInfo(CONSOLE_HANDLE, &info);
    return { info.srWindow.Right - info.srWindow.Left + 1, info.srWindow.Bottom - info.srWindow.Top + 1 };
}

void kl::console::set_size(const Int2& size)
{
    const SMALL_RECT rect = { 0, 0, (SHORT) (size.x - 1), (SHORT) (size.y - 1) };
    SetConsoleWindowInfo(CONSOLE_HANDLE, true, &rect);
}

void kl::console::set_font(const Int2& size, const std::string& font_name)
{
    CONSOLE_FONT_INFOEX cfi = {};
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    cfi.dwFontSize.X = (SHORT) size.x;
    cfi.dwFontSize.Y = (SHORT) size.y;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, convert_string(font_name).c_str());
    SetCurrentConsoleFontEx(CONSOLE_HANDLE, false, &cfi);
}

char kl::console::read()
{
    char input = 0;
    while (_kbhit()) {
        input = (char) _getch();
    }
    return input;
}

void kl::console::wait(const char to_wait_for, const bool echo)
{
    if (echo) {
        if (to_wait_for > 31 && to_wait_for < 127) {
            print("Press '", to_wait_for, "' to continue...");
        }
        else {
            print("Press '", (int) to_wait_for, "' to continue...");
        }
    }
    for (char c = 0; (c = (char) _getch()) != to_wait_for;) {
        print((int) c);
    }
}

char kl::console::wait_for_any(const bool echo)
{
    if (echo) {
        print("Press any key to continue...");
    }
    return (char) _getch();
}

void kl::console::dump(const std::string& data, const Int2& location)
{
    static DWORD ignored = 0;
    WriteConsoleOutputCharacterA(CONSOLE_HANDLE, data.c_str(), (DWORD) data.length(), { (SHORT) location.x, (SHORT) location.y }, &ignored);
}

void kl::console::progress_bar(const std::string& message, int output_y, float percentage)
{
    percentage = std::clamp(percentage, 0.0f, 1.0f);

    const int bar_length = (int)(width() - message.length() - 12);
    const int finish_length = (int) ((float) bar_length * percentage);
    const int empty_length = bar_length - finish_length;

    std::stringstream stream = {};
    stream << "  " << message << " [";
    for (int i = 0; i < finish_length; i++) {
        stream << '#';
    }
    for (int i = 0; i < empty_length; i++) {
        stream << ' ';
    }
    move_cursor({ 0, output_y });

    printf("%s] %3d%% ", stream.str().c_str(), (int) (percentage * 100.0f));
}

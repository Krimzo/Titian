#include "utility/format/console.h"

#include "utility/utility.h"


static const HANDLE console_handle = []
{
    const HANDLE temp_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD console_mode = {};
    GetConsoleMode(temp_handle, &console_mode);
    SetConsoleMode(temp_handle, console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    return temp_handle;
}();

int kl::get()
{
    return std::cin.get();
}

bool kl::warning_check(const bool occured, const std::string& message, const bool wait)
{
    if (occured) {
        print(colors::orange, "Warning: ", message);

        if (wait) {
            console::wait_for_any();
        }

        print<false>(colors::console);
    }
    return occured;
}

void kl::error_check(const bool occured, const std::string& message, const bool wait)
{
    if (occured) {
        print(colors::red, "Error: ", message);

        if (wait) {
            console::wait_for_any();
        }

        print<false>(colors::console);
        exit(1);
    }
}

void kl::console::set_enabled(const bool state)
{
    static HWND console_window = GetConsoleWindow();
    ShowWindow(console_window, state ? SW_SHOW : SW_HIDE);
}

void kl::console::clear()
{
    CONSOLE_SCREEN_BUFFER_INFO console_screen_info = {};
    GetConsoleScreenBufferInfo(console_handle, &console_screen_info);

    DWORD chars_written = {};
    FillConsoleOutputCharacterA(console_handle, ' ', console_screen_info.dwSize.X * console_screen_info.dwSize.Y, {}, &chars_written);

    FillConsoleOutputAttribute(
        console_handle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        console_screen_info.dwSize.X * console_screen_info.dwSize.Y, {}, &chars_written
    );

    move_cursor({});
}

void kl::console::set_cursor_enabled(const bool state)
{
    CONSOLE_CURSOR_INFO cursor_info = {};
    GetConsoleCursorInfo(console_handle, &cursor_info);
    cursor_info.bVisible = state;
    SetConsoleCursorInfo(console_handle, &cursor_info);
}

void kl::console::move_cursor(const int2& position)
{
    SetConsoleCursorPosition(console_handle, { SHORT(position.x), SHORT(position.y) });
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

kl::int2 kl::console::size()
{
    CONSOLE_SCREEN_BUFFER_INFO info = {};
    GetConsoleScreenBufferInfo(console_handle, &info);
    return { info.srWindow.Right - info.srWindow.Left + 1, info.srWindow.Bottom - info.srWindow.Top + 1 };
}

void kl::console::set_size(const int2& size)
{
    const SMALL_RECT rect = { 0, 0, SHORT(size.x - 1), SHORT(size.y - 1) };
    SetConsoleWindowInfo(console_handle, true, &rect);
}

void kl::console::set_font(const int2& size, const std::string& font_name)
{
    CONSOLE_FONT_INFOEX cfi = {};
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    cfi.dwFontSize.X = SHORT(size.x);
    cfi.dwFontSize.Y = SHORT(size.y);
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, strings::to_w_string(font_name).c_str());
    SetCurrentConsoleFontEx(console_handle, false, &cfi);
}

char kl::console::get_input()
{
    char input = 0;
    while (_kbhit()) {
        input = char(_getch());
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
            print("Press '", int(to_wait_for), "' to continue...");
        }
    }

    for (char c = 0; (c = char(_getch())) != to_wait_for;) {
        print(int(c));
    }
}

char kl::console::wait_for_any(const bool echo)
{
    if (echo) {
        print("Press any key to continue...");
    }
    return char(_getch());
}

void kl::console::dump(const std::string& data, const int2& location)
{
    static DWORD ignored = 0;
    WriteConsoleOutputCharacterA(console_handle, data.c_str(), DWORD(data.length()), { SHORT(location.x), SHORT(location.y) }, &ignored);
}

void kl::console::progress_bar(const std::string& message, int output_y, float percentage)
{
    percentage = min(max(percentage, 0.0f), 1.0f);

    const int bar_length = int(width() - message.length() - 12);
    const int finish_length = int(bar_length * percentage);
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

    printf("%s] %3d%% ", stream.str().c_str(), int(percentage * 100.0f));
}

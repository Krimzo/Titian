#pragma once

#include "math/math.h"


namespace kl {
    int get();

    bool warning_check(bool occured, const std::string& message, bool wait = false);
    void error_check(bool occured, const std::string& message, bool wait = true);
}

namespace kl::console {
    void set_enabled(bool state);
    void clear();

    void set_cursor_enabled(bool state);
    void move_cursor(const int2& position);

    int width();
    void set_width(int width);

    int height();
    void set_height(int height);

    int2 size();
    void set_size(const int2& size);

    void set_title(const std::string& text);
    void set_font(const int2& size, const std::string& font_name = "Consolas");

    char get_input();
    void wait(char to_wait_for, bool echo = false);
    char wait_for_any(bool echo = false);

    void dump(const std::string& data, const int2& location = {});

    void progress_bar(const std::string& message, int output_y, float percentage);
}

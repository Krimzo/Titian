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
    void move_cursor(const Int2& position);

    int width();
    void set_width(int width);

    int height();
    void set_height(int height);

    Int2 size();
    void set_size(const Int2& size);

    void set_title(const std::string& text);
    void set_font(const Int2& size, const std::string& font_name = "Consolas");

    char read();
    void wait(char to_wait_for, bool echo = false);
    char wait_for_any(bool echo = false);

    void dump(const std::string& data, const Int2& location = {});
    void progress_bar(const std::string& message, int output_y, float percentage);
}

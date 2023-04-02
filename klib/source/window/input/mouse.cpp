#include "window/input/mouse.h"

#include "window/window.h"


// Window private
void kl::mouse::process() const
{
    left.process();
    middle.process();
    right.process();
}

// User access
bool kl::mouse::is_hidden() const
{
    return hidden_;
}

void kl::mouse::set_hidden(const bool enabled)
{
    if (!hidden_ && enabled) {
        while (show_counter_ > -1) {
            show_counter_ = ShowCursor(false);
        }
    }
    else if (hidden_ && !enabled) {
        while (show_counter_ < 0) {
            show_counter_ = ShowCursor(true);
        }
    }
    hidden_ = enabled;
}

kl::int2 kl::mouse::position(const bool client) const
{
    if (client) {
        return position_;
    }

    POINT client_position = { position_.x, position_.y };
    ClientToScreen(window_, &client_position);
    return { client_position.x, client_position.y };
}

void kl::mouse::set_position(const int2& position, const bool client) const
{
    if (client) {
        POINT client_position = { position.x, position.y };
        ClientToScreen(window_, &client_position);
        SetCursorPos(client_position.x, client_position.y);
    }
    else {
        SetCursorPos(position.x, position.y);
    }
}

kl::float2 kl::mouse::get_normalized_position(const bool client) const
{
    int2 frame_size = screen::size;
    if (client) {
        RECT client_area = {};
        GetClientRect(window_, &client_area);
        frame_size = {
            client_area.right - client_area.left,
            client_area.bottom - client_area.top,
        };
    }

    float2 result = {
        position_.x / (float) frame_size.x,
        (frame_size.y - position_.y) / (float) frame_size.y,
    };
    result *= 2.0f;
    result -= float2(1.0f);
    return result;
}

int kl::mouse::scroll() const
{
    return scroll_;
}

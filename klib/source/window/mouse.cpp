#include "window/mouse.h"


kl::mouse::mouse()
    : left(0), middle(1), right(2)
{}

kl::mouse::~mouse()
{}

void kl::mouse::bind_to_window(const HWND window)
{
    window_ = window;
}

void kl::mouse::update_value(const WPARAM type, const bool state)
{
    for (auto& key : buffer) {
        key.update_value(type, state);
    }
}

void kl::mouse::process() const
{
    for (auto& key : buffer) {
        key.process();
    }
}

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

int kl::mouse::scroll() const
{
    return scroll_;
}

void kl::mouse::update_scroll(const HWND access_check, const int delta_scroll)
{
    if (access_check == window_) {
        scroll_ += delta_scroll;
    }
}

kl::int2 kl::mouse::position(const bool client) const
{
    if (!client) {
        POINT client_position = {position_.x, position_.y};
        ClientToScreen(window_, &client_position);
        return {client_position.x, client_position.y};
    }
    return position_;
}

void kl::mouse::update_position(const HWND access_check, const int2& position)
{
    if (access_check == window_) {
        position_ = position;
    }
}

void kl::mouse::set_position(const int2& position) const
{
    if (window_) {
        POINT client_position = {position.x, position.y};
        ClientToScreen(window_, &client_position);
        SetCursorPos(client_position.x, client_position.y);
    }
}

kl::float2 kl::mouse::get_normalized_position() const
{
    if (window_) {
        RECT client_area = {};
        GetClientRect(window_, &client_area);
        const int2 frame_size = {client_area.right - client_area.left, client_area.bottom - client_area.top};

        float2 result = { float(position_.x) / frame_size.x, float(frame_size.y - position_.y) / frame_size.y };
        result *= 2.0f;
        result -= float2::splash(1);
        
        return result;
    }
    return {};
}

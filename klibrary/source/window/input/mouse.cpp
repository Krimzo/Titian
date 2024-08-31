#include "klibrary.h"


// Window private
void kl::Mouse::process() const
{
    left.process();
    middle.process();
    right.process();
}

// User access
void kl::Mouse::set_hidden(const bool enabled)
{
    if (!m_hidden && enabled) {
        while (m_show_counter > -1) {
            m_show_counter = ShowCursor(false);
        }
    }
    else if (m_hidden && !enabled) {
        while (m_show_counter < 0) {
            m_show_counter = ShowCursor(true);
        }
    }
    m_hidden = enabled;
}

bool kl::Mouse::is_hidden() const
{
    return m_hidden;
}

void kl::Mouse::set_position(const Int2& position)
{
    m_position = position;
    POINT screen_position{ position.x, position.y };
    ClientToScreen(m_window, &screen_position);
    SetCursorPos(screen_position.x, screen_position.y);
}

kl::Int2 kl::Mouse::position() const
{
    return m_position;
}

kl::Float2 kl::Mouse::normalized_position() const
{
    RECT client_area{};
    GetClientRect(m_window, &client_area);
    const Int2 frame_size{
        client_area.right - client_area.left,
        client_area.bottom - client_area.top,
    };
    Float2 result = {
        (float) m_position.x / (float) frame_size.x,
        (float) (frame_size.y - m_position.y) / (float) frame_size.y,
    };
    result *= 2.0f;
    result -= Float2(1.0f);
    return result;
}

int kl::Mouse::scroll() const
{
    return m_scroll;
}

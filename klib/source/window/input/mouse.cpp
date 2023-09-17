#include "klib.h"


// Window private
void kl::Mouse::process() const
{
    left.process();
    middle.process();
    right.process();
}

// User access
bool kl::Mouse::is_hidden() const
{
    return m_hidden;
}

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

void kl::Mouse::set_position(const Int2& position, const bool client)
{
    POINT client_position = { position.x, position.y };
    POINT screen_position = { position.x, position.y };

    if (client) {
        ClientToScreen(m_window, &screen_position);
    }
    else {
        ScreenToClient(m_window, &client_position);
    }

    m_position = { client_position.x, client_position.y };
    SetCursorPos(screen_position.x, screen_position.y);
}

kl::Int2 kl::Mouse::position(const bool client) const
{
    if (client) {
        return m_position;
    }

    POINT client_position = { m_position.x, m_position.y };
    ClientToScreen(m_window, &client_position);
    return { client_position.x, client_position.y };
}

kl::Float2 kl::Mouse::normalized_position(const bool client) const
{
    Int2 frame_size = screen::SIZE;
    if (client) {
        RECT client_area = {};
        GetClientRect(m_window, &client_area);
        frame_size = {
            client_area.right - client_area.left,
            client_area.bottom - client_area.top,
        };
    }

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

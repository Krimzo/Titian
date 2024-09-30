#include "klibrary.h"


void kl::Mouse::set_position(const Int2 position)
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

kl::Float2 kl::Mouse::norm_position() const
{
    RECT client_area{};
    GetClientRect(m_window, &client_area);
    const Int2 frame_size{
        client_area.right - client_area.left,
        client_area.bottom - client_area.top,
    };
    return {
        2.0f * m_position.x / frame_size.x - 1.0f,
        2.0f * (frame_size.y - m_position.y) / frame_size.y - 1.0f,
    };
}

int kl::Mouse::scroll() const
{
    return m_scroll;
}

void kl::Mouse::set_hidden(const bool state)
{
    if (state) {
        while (ShowCursor(false) > -1);
    }
    else {
        while (ShowCursor(true) < 0);
    }
    m_hidden = state;
}

bool kl::Mouse::is_hidden() const
{
    return m_hidden;
}

void kl::Mouse::_reload()
{
	m_scroll = 0;
    for (auto& key : keys) {
		key._reload();
    }
}

void kl::Mouse::_update(const int btn_num, const bool new_state)
{
    keys[btn_num - 1]._update(new_state);
}

void kl::Mouse::_set_window(HWND window)
{
	m_window = window;
}

void kl::Mouse::_update_scroll(const int scroll)
{
    m_scroll = (int16_t) scroll;
}

void kl::Mouse::_update_position(const Int2 position)
{
	m_position = position;
}

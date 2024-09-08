#include "klibrary.h"


bool kl::Key::pressed() const
{
    return m_state == KeyState::PRESSED;
}

bool kl::Key::released() const
{
    return m_state == KeyState::RELEASED;
}

kl::Key::operator bool() const
{
    return m_state == KeyState::DOWN;
}

void kl::Key::_reload()
{
    if (m_state == KeyState::PRESSED) {
		m_state = KeyState::DOWN;
	}
	else if (m_state == KeyState::RELEASED) {
		m_state = KeyState::UP;
	}
}

void kl::Key::_update(const bool new_state)
{
	if (new_state && m_state != KeyState::DOWN) {
		m_state = KeyState::PRESSED;
	}
	else if (!new_state) {
		m_state = KeyState::RELEASED;
	}
}

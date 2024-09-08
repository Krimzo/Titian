#include "klibrary.h"


kl::MouseHook::MouseHook()
{
    m_hook = SetWindowsHookExA(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    assert(m_hook, "Failed to set mouse hook");
    m_self = this;
}

kl::MouseHook::~MouseHook()
{
    UnhookWindowsHookEx(m_hook);
}

void kl::MouseHook::process()
{
    _reload();
    MSG message{};
    GetMessageA(&message, nullptr, 0, 0);
    TranslateMessage(&message);
    DispatchMessageA(&message);
}

LRESULT CALLBACK kl::MouseHook::LowLevelMouseProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    const auto mouse_struct = reinterpret_cast<const MSLLHOOKSTRUCT*>(lParam);
    if (nCode == HC_ACTION) {
        switch (wParam)
        {
        case WM_LBUTTONDOWN:
            m_self->_update(1, true);
            break;
        case WM_LBUTTONUP:
            m_self->_update(1, false);
            break;

        case WM_RBUTTONDOWN:
            m_self->_update(2, true);
            break;
        case WM_RBUTTONUP:
            m_self->_update(2, false);
            break;

        case WM_MBUTTONDOWN:
            m_self->_update(3, true);
            break;
        case WM_MBUTTONUP:
            m_self->_update(3, false);
            break;

        case WM_XBUTTONDOWN:
            m_self->_update(GET_XBUTTON_WPARAM(mouse_struct->mouseData) + 3, true);
            break;
        case WM_XBUTTONUP:
            m_self->_update(GET_XBUTTON_WPARAM(mouse_struct->mouseData) + 3, false);
            break;

        case WM_MOUSEMOVE:
            m_self->_update_position({ mouse_struct->pt.x, mouse_struct->pt.y });
            break;
        case WM_MOUSEWHEEL:
            m_self->_update_scroll(GET_WHEEL_DELTA_WPARAM(mouse_struct->mouseData) / WHEEL_DELTA);
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

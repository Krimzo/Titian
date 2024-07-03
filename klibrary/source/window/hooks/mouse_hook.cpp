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

void kl::MouseHook::process(const bool wait)
{
    MSG message{};
    if (wait) {
        GetMessageA(&message, nullptr, 0, 0);
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    else {
        while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
    Mouse::process();
}

LRESULT CALLBACK kl::MouseHook::LowLevelMouseProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        switch (wParam)
        {
        case WM_LBUTTONDOWN:
            m_self->left.update(true);
            break;
        case WM_LBUTTONUP:
            m_self->left.update(false);
            break;

        case WM_MBUTTONDOWN:
            m_self->middle.update(true);
            break;
        case WM_MBUTTONUP:
            m_self->middle.update(false);
            break;

        case WM_RBUTTONDOWN:
            m_self->right.update(true);
            break;
        case WM_RBUTTONUP:
            m_self->right.update(false);
            break;

        case WM_MOUSEMOVE:
        {
            auto mouse_struct = reinterpret_cast<const MSLLHOOKSTRUCT*>(lParam);
            const Int2 pos{ (int) mouse_struct->pt.x, (int) mouse_struct->pt.y };
            m_self->m_position = pos;
            for (const auto& func : m_self->on_move) {
                func(pos);
            }
        }
        break;
        case WM_MOUSEWHEEL:
        {
            auto mouse_struct = reinterpret_cast<const MSLLHOOKSTRUCT*>(lParam);
            const int delta = (int) (GET_WHEEL_DELTA_WPARAM(mouse_struct->mouseData) / WHEEL_DELTA);
            m_self->m_scroll += delta;
            for (const auto& func : m_self->on_scroll) {
                func(m_self->m_scroll);
            }
        }
        break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

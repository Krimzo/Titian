#include "klibrary.h"


kl::KeyboardHook::KeyboardHook()
{
    m_hook = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    assert(m_hook, "Failed to set keyboard hook");
    m_self = this;
}

kl::KeyboardHook::~KeyboardHook()
{
    UnhookWindowsHookEx(m_hook);
}

void kl::KeyboardHook::process()
{
    _reload();
    MSG message{};
    GetMessageA(&message, nullptr, 0, 0);
    TranslateMessage(&message);
    DispatchMessageA(&message);
}

LRESULT CALLBACK kl::KeyboardHook::LowLevelKeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    const auto keyboard_struct = reinterpret_cast<const KBDLLHOOKSTRUCT*>(lParam);
    if (nCode == HC_ACTION) {
        switch (wParam)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            m_self->_update(keyboard_struct->vkCode, true);
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            m_self->_update(keyboard_struct->vkCode, false);
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

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

void kl::KeyboardHook::process(const bool wait)
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
    Keyboard::process();
}

LRESULT CALLBACK kl::KeyboardHook::LowLevelKeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        switch (wParam)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            m_self->update(reinterpret_cast<const KBDLLHOOKSTRUCT*>(lParam)->vkCode, true);
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            m_self->update(reinterpret_cast<const KBDLLHOOKSTRUCT*>(lParam)->vkCode, false);
            break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

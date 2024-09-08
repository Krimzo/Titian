#pragma once

#include "window/input/keyboard.h"


namespace kl {
    class KeyboardHook : NoCopy, public Keyboard
    {
    public:
        KeyboardHook();
        ~KeyboardHook();

        void process();

    private:
        static inline KeyboardHook* m_self = nullptr;
        static inline HHOOK m_hook = nullptr;

        static LRESULT CALLBACK LowLevelKeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam);
    };
}

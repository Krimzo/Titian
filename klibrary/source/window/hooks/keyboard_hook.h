#pragma once

#include "window/input/keyboard.h"


namespace kl {
    struct KeyboardHook : NoCopy, Keyboard
    {
        KeyboardHook();
        ~KeyboardHook();

        void process();

    private:
        static inline KeyboardHook* m_self = nullptr;
        static inline HHOOK m_hook = nullptr;

        static LRESULT CALLBACK LowLevelKeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam);
    };
}

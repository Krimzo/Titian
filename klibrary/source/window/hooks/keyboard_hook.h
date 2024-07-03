#pragma once

#include "window/input/keyboard.h"


namespace kl {
    class KeyboardHook : public Keyboard
    {
    public:
        KeyboardHook();
        ~KeyboardHook();

        KeyboardHook(const KeyboardHook&) = delete;
        KeyboardHook(KeyboardHook&&) = delete;

        void operator=(const KeyboardHook&) = delete;
        void operator=(KeyboardHook&&) = delete;

        void process(bool wait = true);

    private:
        static inline KeyboardHook* m_self = nullptr;
        static inline HHOOK m_hook = nullptr;

        static LRESULT CALLBACK LowLevelKeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam);
    };
}

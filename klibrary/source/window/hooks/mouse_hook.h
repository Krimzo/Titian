#pragma once

#include "window/input/mouse.h"


namespace kl {
    struct MouseHook : NoCopy, Mouse
    {
        MouseHook();
        ~MouseHook();

        void process();

    private:
        static inline MouseHook* m_self = nullptr;
        static inline HHOOK m_hook = nullptr;

        static LRESULT CALLBACK LowLevelMouseProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam);
    };
}

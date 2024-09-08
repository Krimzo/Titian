#pragma once

#include "window/input/mouse.h"


namespace kl {
    class MouseHook : NoCopy, public Mouse
    {
    public:
        MouseHook();
        ~MouseHook();

        void process();

    private:
        static inline MouseHook* m_self = nullptr;
        static inline HHOOK m_hook = nullptr;

        static LRESULT CALLBACK LowLevelMouseProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam);
    };
}

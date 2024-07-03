#pragma once

#include "window/input/mouse.h"


namespace kl {
    class MouseHook : public Mouse
    {
    public:
        MouseHook();
        ~MouseHook();

        MouseHook(const MouseHook&) = delete;
        MouseHook(MouseHook&&) = delete;

        void operator=(const MouseHook&) = delete;
        void operator=(MouseHook&&) = delete;

        void process(bool wait = true);

    private:
        static inline MouseHook* m_self = nullptr;
        static inline HHOOK m_hook = nullptr;

        static LRESULT CALLBACK LowLevelMouseProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam);
    };
}

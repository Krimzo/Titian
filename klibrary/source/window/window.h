#pragma once

// Input
#include "window/input/key.h"
#include "window/input/keyboard.h"
#include "window/input/mouse.h"

// Draw
#include "window/draw/frame_buffer.h"


namespace kl::screen {
    inline const Int2 SIZE = {
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
    };
}

namespace kl {
    class Window
    {
        std::string m_name = {};

        HINSTANCE m_instance = nullptr;
        HWND m_window = nullptr;
        HDC m_device_context = nullptr;

        bool m_in_fullscreen = false;
        bool m_resizeable = true;
        LONG m_window_style = NULL;
        LONG m_window_ex_style = NULL;
        WINDOWPLACEMENT m_placement = {};

        // System
        LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) const;
        void handle_message(const MSG& message);

    public:
        std::vector<std::function<void(Int2)>> on_resize = {};
		std::vector<std::function<void(Int2)>> on_move = {};

        Keyboard keyboard = {};
        Mouse mouse = {};

        // Creation
        Window(const std::string& name, const Int2& size);
        virtual ~Window();

        Window(const Window&) = delete;
        Window(const Window&&) = delete;

        void operator=(const Window&) = delete;
        void operator=(const Window&&) = delete;

        // Methods
        operator HWND() const;

        bool process(bool wait = true);

        bool is_open() const;
        void close() const;

        bool is_resizeable() const;
        void set_resizeable(bool enabled);

        void maximize() const;
        void minimize() const;
        void restore() const;

        bool in_fullscreen() const;
        void set_fullscreen(bool enabled);

        Int2 position(bool client = false) const;
        void set_position(const Int2& position) const;

        int width() const;
        void set_width(int width) const;

        int height() const;
        void set_height(int height) const;

        Int2 size(bool client = true) const;
        void resize(const Int2& size, bool client = true) const;

        float aspect_ratio() const;
        Int2 frame_center() const;

        void set_title(const std::string& data) const;
        bool set_icon(const std::string& filepath) const;

        void draw_pixel_data(const Color* data, const Int2& size, const Int2& position = {}) const;
        void draw_image(const Image& image, const Int2& position = {}) const;

        void notify() const;
    };
}

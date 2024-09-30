#pragma once

#include "window/input/key.h"
#include "window/input/keyboard.h"
#include "window/input/mouse.h"
#include "window/hooks/keyboard_hook.h"
#include "window/hooks/mouse_hook.h"


namespace kl::screen {
    inline const Int2 SIZE = {
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
    };
}

namespace kl {
    struct Window : NoCopy
    {
        Keyboard keyboard;
        Mouse mouse;

        std::vector<std::function<void(Int2)>> on_resize;
		std::vector<std::function<void(Int2)>> on_move;

        Window(const std::string_view& name);
        virtual ~Window();

        operator HWND() const;

        bool process();

        bool is_open() const;
        void close() const;

        bool is_resizeable() const;
        void set_resizeable(bool enabled);

        int style() const;
        void add_style(int style);
        void remove_style(int style);

        void maximize() const;
        void minimize() const;
        void restore() const;

        bool is_maximized() const;
        bool is_minimized() const;
        bool is_restored() const;
        bool is_focused() const;

        bool in_fullscreen() const;
        void set_fullscreen(bool enabled);

        Int2 position() const;
        void set_position(Int2 position) const;

        int width() const;
        void set_width(int width) const;

        int height() const;
        void set_height(int height) const;

        Int2 size() const;
        void resize(Int2 size) const;

        float aspect_ratio() const;
        Int2 frame_center() const;

        float dpi() const;
        float pixels_to_dips(float value) const;
        float dips_to_pixels(float value) const;

        void set_title(const std::string_view& data) const;
        bool set_icon(const std::string_view& filepath) const;

        void draw_pixel_data(const RGB* data, Int2 size, Int2 position = {}) const;
        void draw_image(const Image& image, Int2 position = {}) const;

        void set_dark_mode(bool enabled) const;
        void notify() const;

    private:
        std::string m_name;

        HINSTANCE m_instance = nullptr;
        HWND m_window = nullptr;
        HDC m_device_context = nullptr;

        WINDOWPLACEMENT m_placement = {};
        LONG m_window_ex_style = NULL;
        LONG m_window_style = NULL;
        bool m_resizeable = true;
        bool m_in_fullscreen = false;

        LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) const;
        void handle_message(const MSG& message);
    };
}

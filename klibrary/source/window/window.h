#pragma once

#include "window/input/key.h"
#include "window/input/keyboard.h"
#include "window/input/mouse.h"
#include "window/hooks/keyboard_hook.h"
#include "window/hooks/mouse_hook.h"


namespace kl {
    inline const Int2 SCREEN_SIZE = {
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
        ~Window();

        HWND ptr() const;
        bool process();

        bool active() const;
        void close() const;

        bool resizeable() const;
        void set_resizeable(bool enabled);

        bool fullscreened() const;
        void set_fullscreen(bool enabled);

        LONG style() const;
        void add_style(LONG style_val);
        void remove_style(LONG style_val);

        bool focused() const;

        void maximize() const;
        void minimize() const;
        void restore() const;

        bool maximized() const;
        bool minimized() const;
        bool restored() const;

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

        LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) const;
        void handle_message(const MSG& message);
    };
}

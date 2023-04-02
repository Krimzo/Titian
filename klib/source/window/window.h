#pragma once

// Input
#include "window/input/key.h"
#include "window/input/keyboard.h"
#include "window/input/mouse.h"

// Draw
#include "window/draw/frame_buffer.h"
#include "memory/memory.h"


namespace kl::screen {
    inline const int2 size = {
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
    };
}

namespace kl {
    class window
    {
        std::string name_ = {};

        HINSTANCE instance_ = nullptr;
        HWND window_ = nullptr;
        HDC device_context_ = nullptr;

        bool in_fullscreen_ = false;
        bool resizeable_ = true;
        LONG window_style_ = NULL;
        LONG window_ex_style_ = NULL;
        WINDOWPLACEMENT placement_ = {};

        // System
        LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) const;
        void handle_message(const MSG& message);

    public:
        std::vector<std::function<void(int2)>> on_resize = {};
        keyboard keyboard = {};
        mouse mouse = {};

        // Creation
        window(const int2& size, const std::string& name);
        ~window();

        window(const window&) = delete;
        window(const window&&) = delete;

        void operator=(const window&) = delete;
        void operator=(const window&&) = delete;

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

        int2 position(bool client = false) const;
        void set_position(const int2& position) const;

        int width() const;
        void set_width(int width) const;

        int height() const;
        void set_height(int height) const;

        int2 size(bool client = true) const;
        void resize(const int2& size, bool client = true) const;

        float get_aspect_ratio() const;
        int2 get_frame_center() const;

        void set_title(const std::string& data) const;
        bool set_icon(const std::string& filepath) const;

        void draw_pixel_data(const color* data, const int2& size, const int2& position = {}) const;
        void draw_image(const image& image, const int2& position = {}) const;

        void notify() const;
    };
}

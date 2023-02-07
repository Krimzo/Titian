#include "window/system_handler.h"

#include "window/window.h"

#ifdef KL_USING_IMGUI
#include "imgui.h"
#endif


#ifdef KL_USING_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT CALLBACK kl::window::window_procedure(const HWND window_handle, const UINT message, const WPARAM w_param, const LPARAM l_param) const
{
    if (message == WM_SIZE) {
        on_resize({ LOWORD(l_param), HIWORD(l_param) });
    }
    return DefWindowProcA(window_handle, message, w_param, l_param);
}

void kl::window::handle_message(const MSG& message)
{
#ifdef KL_USING_IMGUI
    TranslateMessage(&message);
    if (ImGui_ImplWin32_WndProcHandler(message.hwnd, message.message, message.wParam, message.lParam)) {
        return;
    }
#endif

    switch (message.message) {
#ifdef KL_USING_IMGUI
    case WM_CHAR:
        if (*((short*) &message.lParam) > 1) {
            ImGui::GetIO().AddInputCharacter(int(message.wParam));
        }
        break;
#endif

    case WM_KEYDOWN:
        keyboard.update_value(message.wParam, true);
        break;

    case WM_KEYUP:
        keyboard.update_value(message.wParam, false);
        break;

    case WM_LBUTTONDOWN:
        mouse.left.update_value(0, true);
        break;

    case WM_LBUTTONUP:
        mouse.left.update_value(0, false);
        break;

    case WM_MBUTTONDOWN:
        mouse.middle.update_value(1, true);
        break;

    case WM_MBUTTONUP:
        mouse.middle.update_value(1, false);
        break;

    case WM_RBUTTONDOWN:
        mouse.right.update_value(2, true);
        break;

    case WM_RBUTTONUP:
        mouse.right.update_value(2, false);
        break;

    case WM_MOUSEMOVE:
        mouse.update_position(window_, { GET_X_LPARAM(message.lParam), GET_Y_LPARAM(message.lParam) });
        break;

    case WM_MOUSEWHEEL:
        mouse.update_scroll(window_, GET_WHEEL_DELTA_WPARAM(message.wParam) / 120);
        break;

    default:
        DispatchMessageA(&message);
        break;
    }
}

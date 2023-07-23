#pragma once

#include "klib.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <imgui.h>
#include <ImGuizmo.h>


namespace GUI {
    void set_drag_data(const char* id, const void* data, size_t data_size, const kl::dx::shader_view& texture = nullptr);

    template<typename T>
    void set_drag_data(const char* id, const T& data, const kl::dx::shader_view& texture = nullptr)
    {
        set_drag_data(id, &data, sizeof(T), texture);
    }

    template<typename T>
    bool get_drag_data(const char* id, T& out_data)
    {
        bool state = false;
        if (ImGui::BeginDragDropTarget()) {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(id);
            if (payload->DataSize == sizeof(T)) {
                memcpy(&out_data, payload->Data, sizeof(T));
                state = true;
            }
            ImGui::EndDragDropTarget();
        }
        return state;
    }
}

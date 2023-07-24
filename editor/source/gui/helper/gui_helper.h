#pragma once

#include "klib.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <imgui.h>
#include <ImGuizmo.h>


namespace GUI::popup {
    void on_window(const std::string& id, const std::function<void()>& callback);
    void on_item(const std::string& id, const std::function<void()>& callback);
    void close();
}

namespace GUI::drag_drop {
    inline std::unordered_map<std::string, std::any> _data = {};

    template<typename T>
    void write_data(const std::string& id, const T& data, const kl::dx::shader_view& texture = nullptr)
    {
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(id.c_str(), nullptr, 0);
            if (texture) ImGui::Image(texture.Get(), { 50.0f, 50.0f });
            _data[id] = { data };
            ImGui::EndDragDropSource();
        }
    }

    template<typename T>
    std::optional<T> read_data(const std::string& id)
    {
        std::optional<T> result = {};
        if (ImGui::BeginDragDropTarget()) {
            if (ImGui::AcceptDragDropPayload(id.c_str())) {
                try {
                    const std::any data = _data[id];
                    result = std::any_cast<T>(data);
                }
                catch (const std::exception&) {
                }
            }
            ImGui::EndDragDropTarget();
        }
        return result;
    }
}

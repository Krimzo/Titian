#pragma once

#include "klibrary.h"


namespace titian::_priv {
    inline std::unordered_map<std::string, std::any> DRAG_DROP_DATA = {};
    inline std::unordered_map<std::string, char[101]> INPUT_CONTINUOUS_DATA = {};
    inline std::unordered_map<std::string, char[101]> INPUT_WAITED_DATA = {};
}

namespace titian {
    inline void gui_colored_text(const std::string_view& message, const kl::Float4& color)
    {
        ImGui::TextColored(reinterpret_cast<const ImVec4&>(color), message.data());
    }

    inline std::pair<ImVec2, ImVec2> gui_window_rect()
    {
        const ImVec2 content_region = ImGui::GetContentRegionAvail();
        const ImVec2 win_content_min = ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
        const ImVec2 win_content_max = win_content_min + content_region;
        return { win_content_min, win_content_max };
    }

    inline float gui_calculate_item_with(const std::string_view& label)
    {
        return ImGui::CalcTextSize(label.data()).x;
    }

    inline void gui_align_horizontally(const float width, const float alignment)
    {
        const float available = ImGui::GetContentRegionAvail().x;
        const float offset = (available - width) * alignment;
        if (offset > 0.0f) {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
        }
    }

    inline std::string gui_input_continuous(const std::string& id)
    {
        auto& buffer = _priv::INPUT_CONTINUOUS_DATA[id];
        const int buffer_capacity = static_cast<int>(std::size(buffer) - 1);
        ImGui::InputText(id.c_str(), buffer, buffer_capacity);
        return { { buffer } };
    }

    inline std::optional<std::string> gui_input_waited(const std::string& id, const std::string_view& to_copy)
    {
        auto& buffer = _priv::INPUT_WAITED_DATA[id];
        const int buffer_capacity = static_cast<int>(std::size(buffer) - 1);

        // Copy/clear buffer
        for (int i = 0; i < buffer_capacity; i++) {
            const char value = (i < to_copy.size()) ? to_copy[i] : 0;
            buffer[i] = value;
        }

        // Render input
        if (ImGui::InputText(id.c_str(), buffer, buffer_capacity, ImGuiInputTextFlags_EnterReturnsTrue)) {
            const std::string result = { buffer };
            memset(buffer, 0, std::size(buffer));
            return { result };
        }
        return {};
    }

    template<typename T>
    void gui_set_drag_drop(const std::string& id, const T& data, const kl::dx::ShaderView& texture = nullptr)
    {
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(id.c_str(), nullptr, 0);
            if (texture) {
                ImGui::Image(texture.Get(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            }
            _priv::DRAG_DROP_DATA[id] = std::any{ data };
            ImGui::EndDragDropSource();
        }
    }

    template<typename T>
    std::optional<T> gui_get_drag_drop(const std::string& id)
    {
        std::optional<T> result = {};
        if (ImGui::BeginDragDropTarget()) {
            if (ImGui::AcceptDragDropPayload(id.c_str())) {
                const std::any data = _priv::DRAG_DROP_DATA[id];
                result = std::any_cast<T>(data);
                _priv::DRAG_DROP_DATA.erase(id);
            }
            ImGui::EndDragDropTarget();
        }
        return result;
    }
}

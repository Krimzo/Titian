#pragma once

#include "klibrary.h"


namespace titian {
    namespace _priv {
        inline std::unordered_map<std::string, std::any> DRAG_DROP_DATA = {};
        inline std::unordered_map<std::string, std::string> INPUT_CONTINUOUS_DATA = {};
        inline std::unordered_map<std::string, std::string> INPUT_WAITED_DATA = {};
    }

    inline const std::string FILE_EXTENSION_MESH = ".obj";
    inline const std::string FILE_EXTENSION_JPG = ".jpg";
    inline const std::string FILE_EXTENSION_PNG = ".png";
    inline const std::string FILE_EXTENSION_NATIVE_SCRIPT = ".dll";
    inline const std::string FILE_EXTENSION_INTER_SCRIPT = ".chai";
    inline const std::string FILE_EXTENSION_SHADER = ".hlsl";
    inline const std::string FILE_EXTENSION_SCENE = ".titian";

    inline const std::string DRAG_FILE_ID = "DragFileID";
    inline const std::string DRAG_DIR_ID = "DragDirID";

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
        ImGui::InputText(id.c_str(), &buffer);
        return buffer;
    }

    inline std::optional<std::string> gui_input_waited(const std::string& id, const std::string_view& to_copy)
    {
        auto& buffer = _priv::INPUT_WAITED_DATA[id];
        buffer = to_copy;

        if (ImGui::InputText(id.c_str(), &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
            const std::string result = buffer;
            buffer.clear();
            return { result };
        }
        return {};
    }

    template<typename T>
    void gui_set_drag_drop(const std::string& id, const T& data, const kl::dx::ShaderView& texture = nullptr)
    {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, {});
        ImGui::PushStyleColor(ImGuiCol_Border, {});
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(id.c_str(), nullptr, 0);
            _priv::DRAG_DROP_DATA[id] = std::any{ data };
            if (texture) {
                ImGui::Image(texture.Get(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            }
            ImGui::EndDragDropSource();
        }
		ImGui::PopStyleColor(2);
    }

    template<typename T>
    std::optional<T> gui_get_drag_drop(const std::string& id)
    {
        std::optional<T> result{};
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

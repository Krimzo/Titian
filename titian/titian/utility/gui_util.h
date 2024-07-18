#pragma once

#include "klibrary.h"


namespace titian {
    namespace _priv {
        inline std::unordered_map<std::string, std::any> DRAG_DROP_DATA = {};
        inline std::unordered_map<std::string, std::string> INPUT_CONTINUOUS_DATA = {};
        inline std::unordered_map<std::string, std::string> INPUT_WAITED_DATA = {};
    }

    inline const std::string FILE_EXTENSION_OBJ = ".obj";
    inline const std::string FILE_EXTENSION_GLB = ".glb";
    inline const std::string FILE_EXTENSION_FBX = ".fbx";

    inline const std::string FILE_EXTENSION_JPG = ".jpg";
    inline const std::string FILE_EXTENSION_PNG = ".png";
    inline const std::string FILE_EXTENSION_BMP = ".bmp";

    inline const std::string FILE_EXTENSION_DLL = ".dll";
    inline const std::string FILE_EXTENSION_CHAI = ".chai";

    inline const std::string FILE_EXTENSION_HLSL = ".hlsl";
    inline const std::string FILE_EXTENSION_TITIAN = ".titian";

    inline const std::string DRAG_FILE_ID = "DragFileID";
    inline const std::string DRAG_DIR_ID = "DragDirID";

    inline void gui_colored_text(const std::string_view& message, const kl::Float4& color)
    {
        imgui::TextColored(reinterpret_cast<const ImVec4&>(color), message.data());
    }

    inline std::pair<ImVec2, ImVec2> gui_window_rect()
    {
        const ImVec2 content_region = imgui::GetContentRegionAvail();
        const ImVec2 win_content_min = imgui::GetWindowPos() + imgui::GetWindowContentRegionMin();
        const ImVec2 win_content_max = win_content_min + content_region;
        return { win_content_min, win_content_max };
    }

    inline float gui_calculate_item_with(const std::string_view& label)
    {
        return imgui::CalcTextSize(label.data()).x;
    }

    inline void gui_align_horizontally(const float width, const float alignment)
    {
        const float available = imgui::GetContentRegionAvail().x;
        const float offset = (available - width) * alignment;
        if (offset > 0.0f) {
            imgui::SetCursorPosX(imgui::GetCursorPosX() + offset);
        }
    }

    inline std::string gui_input_continuous(const std::string& id)
    {
        auto& buffer = _priv::INPUT_CONTINUOUS_DATA[id];
        imgui::InputText(id.c_str(), &buffer);
        return buffer;
    }

    inline std::optional<std::string> gui_input_waited(const std::string& id, const std::string_view& to_copy)
    {
        auto& buffer = _priv::INPUT_WAITED_DATA[id];
        buffer = to_copy;

        if (imgui::InputText(id.c_str(), &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
            const std::string result = buffer;
            buffer.clear();
            return { result };
        }
        return {};
    }

    template<typename T>
    void gui_set_drag_drop(const std::string& id, const T& data, const kl::dx::ShaderView& texture = nullptr)
    {
        imgui::PushStyleColor(ImGuiCol_PopupBg, {});
        imgui::PushStyleColor(ImGuiCol_Border, {});
        if (imgui::BeginDragDropSource()) {
            imgui::SetDragDropPayload(id.c_str(), nullptr, 0);
            _priv::DRAG_DROP_DATA[id] = std::any{ data };
            if (texture) {
                imgui::Image(texture.Get(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            }
            imgui::EndDragDropSource();
        }
		imgui::PopStyleColor(2);
    }

    template<typename T>
    std::optional<T> gui_get_drag_drop(const std::string& id)
    {
        std::optional<T> result{};
        if (imgui::BeginDragDropTarget()) {
            if (imgui::AcceptDragDropPayload(id.c_str())) {
                const std::any data = _priv::DRAG_DROP_DATA[id];
                result = std::any_cast<T>(data);
                _priv::DRAG_DROP_DATA.erase(id);
            }
            imgui::EndDragDropTarget();
        }
        return result;
    }
}

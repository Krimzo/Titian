#pragma once

#include "standard.h"


namespace titian {
    inline StringMap<Any> _DRAG_DROP_DATA;
    inline StringMap<String> _INPUT_CONTINUOUS_DATA;
    inline StringMap<String> _INPUT_WAITED_DATA;

    inline const String FILE_EXTENSION_OBJ = ".obj";
    inline const String FILE_EXTENSION_GLB = ".glb";
    inline const String FILE_EXTENSION_FBX = ".fbx";

    inline const String FILE_EXTENSION_JPG = ".jpg";
    inline const String FILE_EXTENSION_PNG = ".png";
    inline const String FILE_EXTENSION_BMP = ".bmp";

    inline const String FILE_EXTENSION_DLL = ".dll";
    inline const String FILE_EXTENSION_CHAI = ".chai";

    inline const String FILE_EXTENSION_HLSL = ".hlsl";
    inline const String FILE_EXTENSION_TITIAN = ".titian";
    inline const String FILE_EXTENSION_JSON = ".json";

    inline const String DRAG_FILE_ID = "DragFileID";
    inline const String DRAG_DIR_ID = "DragDirID";

    inline void gui_colored_text(const StringView& message, const Float4& color)
    {
        im::TextColored(reinterpret_cast<const ImVec4&>(color), message.data());
    }

    inline Pair<ImVec2, ImVec2> gui_window_rect()
    {
        const ImVec2 content_region = im::GetContentRegionAvail();
        const ImVec2 win_content_min = im::GetWindowPos() + im::GetWindowContentRegionMin();
        const ImVec2 win_content_max = win_content_min + content_region;
        return { win_content_min, win_content_max };
    }

    inline float gui_calculate_item_with(const StringView& label)
    {
        return im::CalcTextSize(label.data()).x;
    }

    inline void gui_align_horizontally(const float width, const float alignment)
    {
        const float available = im::GetContentRegionAvail().x;
        const float offset = (available - width) * alignment;
        if (offset > 0.0f) {
            im::SetCursorPosX(im::GetCursorPosX() + offset);
        }
    }

    inline String gui_input_continuous(const String& id)
    {
        auto& buffer = _INPUT_CONTINUOUS_DATA[id];
        im::InputText(id.data(), &buffer);
        return buffer;
    }

    inline Optional<String> gui_input_waited(const String& id, const StringView& to_copy)
    {
        auto& buffer = _INPUT_WAITED_DATA[id];
        buffer = to_copy;
        if (im::InputText(id.data(), &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
            const String result = buffer;
            buffer.clear();
            return { result };
        }
        return {};
    }

    template<typename T>
    void gui_set_drag_drop(const String& id, const T& data, const dx::ShaderView& texture = nullptr)
    {
        im::PushStyleColor(ImGuiCol_PopupBg, {});
        im::PushStyleColor(ImGuiCol_Border, {});
        if (im::BeginDragDropSource()) {
            im::SetDragDropPayload(id.data(), nullptr, 0);
            _DRAG_DROP_DATA[id] = Any{ data };
            if (texture) {
                im::Image(texture.get(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            }
            im::EndDragDropSource();
        }
		im::PopStyleColor(2);
    }

    template<typename T>
    Optional<T> gui_get_drag_drop(const StringView& id)
    {
        auto it = _DRAG_DROP_DATA.find(id);
        if (it == _DRAG_DROP_DATA.end())
            return std::nullopt;

        Optional<T> result;
        if (im::BeginDragDropTarget()) {
            if (im::AcceptDragDropPayload(id.data())) {
                result = std::any_cast<T>(it->second);
                _DRAG_DROP_DATA.erase(it);
            }
            im::EndDragDropTarget();
        }
        return result;
    }
}

#pragma once

#include "standard.h"


namespace titian {
    inline StringMap<Any> _DRAG_DROP_DATA;
    inline StringMap<String> _INPUT_CONTINUOUS_DATA;
    inline StringMap<String> _INPUT_WAITED_DATA;

    inline constexpr StringRef FILE_EXTENSION_OBJ = ".obj";
    inline constexpr StringRef FILE_EXTENSION_GLB = ".glb";
    inline constexpr StringRef FILE_EXTENSION_FBX = ".fbx";

    inline constexpr StringRef FILE_EXTENSION_JPG = ".jpg";
    inline constexpr StringRef FILE_EXTENSION_PNG = ".png";
    inline constexpr StringRef FILE_EXTENSION_BMP = ".bmp";

    inline constexpr StringRef FILE_EXTENSION_HLSL = ".hlsl";

    inline constexpr StringRef FILE_EXTENSION_DLL = ".dll";
    inline constexpr StringRef FILE_EXTENSION_LUA = ".lua";

    inline constexpr StringRef FILE_EXTENSION_JSON = ".json";
    inline constexpr StringRef FILE_EXTENSION_TITIAN = ".titian";

    inline constexpr StringRef DRAG_FILE_ID = "DragFileID";
    inline constexpr StringRef DRAG_DIR_ID = "DragDirID";

    inline constexpr StringRef DRAG_BASIC_ENTITY_ID = "BasicEntityID";
    inline constexpr StringRef DRAG_ANIMATION_ENTITY_ID = "AnimationEntityID";

    inline constexpr StringRef DRAG_ENTITY_ENTITY = "entity";
    inline constexpr StringRef DRAG_ENTITY_CAMERA = "camera";
    inline constexpr StringRef DRAG_ENTITY_AMBIENT = "ambient";
    inline constexpr StringRef DRAG_ENTITY_DIRECTIONAL = "directional";
}

namespace titian {
    void gui_colored_text(const StringRef& message, const Float4& color);
    Pair<ImVec2, ImVec2> gui_window_rect();
    float gui_calculate_item_with(const StringRef& label);
    void gui_align_horizontally(float width, float alignment);
    String gui_input_continuous(const String& id);
    Opt<String> gui_input_waited(const String& id, const StringRef& to_copy);
}

namespace titian {
    template<typename T>
    void gui_set_drag_drop(const StringRef& id, const T& data, const dx::ShaderView& texture = nullptr)
    {
        im::PushStyleColor(ImGuiCol_PopupBg, {});
        im::PushStyleColor(ImGuiCol_Border, {});
        if (im::BeginDragDropSource()) {
            im::SetDragDropPayload(id.data(), nullptr, 0);
            _DRAG_DROP_DATA[String{id}] = data;
            if (texture) {
                im::Image(texture.get(), { 50.0f, 50.0f });
            }
            im::EndDragDropSource();
        }
		im::PopStyleColor(2);
    }

    template<typename T>
    Opt<T> gui_get_drag_drop(const StringRef& id)
    {
        auto it = _DRAG_DROP_DATA.find(id);
        if (it == _DRAG_DROP_DATA.end())
            return std::nullopt;

        Opt<T> result;
        if (im::BeginDragDropTarget()) {
            if (im::AcceptDragDropPayload(id.data())) {
                result = std::any_cast<const T&>(it->second);
                _DRAG_DROP_DATA.erase(it);
            }
            im::EndDragDropTarget();
        }
        return result;
    }
}

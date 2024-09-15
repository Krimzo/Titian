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

    inline const String FILE_EXTENSION_HLSL = ".hlsl";

    inline const String FILE_EXTENSION_DLL = ".dll";
    inline const String FILE_EXTENSION_LUA = ".lua";

    inline const String FILE_EXTENSION_JSON = ".json";
    inline const String FILE_EXTENSION_TITIAN = ".titian";

    inline const String DRAG_FILE_ID = "DragFileID";
    inline const String DRAG_DIR_ID = "DragDirID";

    inline const String DRAG_BASIC_ENTITY_ID = "BasicEntityID";
    inline const String DRAG_ANIMATION_ENTITY_ID = "AnimationEntityID";

    inline const String DRAG_ENTITY_ENTITY = "entity";
    inline const String DRAG_ENTITY_CAMERA = "camera";
    inline const String DRAG_ENTITY_AMBIENT = "ambient";
    inline const String DRAG_ENTITY_POINT = "point";
    inline const String DRAG_ENTITY_DIRECTIONAL = "directional";
}

namespace titian {
    void gui_colored_text(const StringView& message, const Float4& color);
    Pair<ImVec2, ImVec2> gui_window_rect();
    float gui_calculate_item_with(const StringView& label);
    void gui_align_horizontally(float width, float alignment);
    String gui_input_continuous(const String& id);
    Optional<String> gui_input_waited(const String& id, const StringView& to_copy);
}

namespace titian {
    template<typename T>
    void gui_set_drag_drop(const String& id, const T& data, const dx::ShaderView& texture = nullptr)
    {
        im::PushStyleColor(ImGuiCol_PopupBg, {});
        im::PushStyleColor(ImGuiCol_Border, {});
        if (im::BeginDragDropSource()) {
            im::SetDragDropPayload(id.data(), nullptr, 0);
            _DRAG_DROP_DATA[id] = Any{ data };
            if (texture) {
                im::Image(texture.get(), { 50.0f, 50.0f });
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

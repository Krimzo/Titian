#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "render/render_layer.h"
#include "serialization/serialization.h"


namespace titian {
    struct GUISectionViewport : GUISection
	{
        GUISectionViewport();

        void render_gui() override;

    private:
        Map<ImGuiKey, bool> m_last_key_states;
        Optional<Int2> m_rect_selection_first;

        Int2 window_mouse_position() const;
        Float2 window_to_ndc(Int2 coords) const;

        Set<uint32_t> read_id_texture(Int2 first_coords, Int2 second_coords);
        Optional<Float3> read_depth_texture(Int2 coords);

        void handle_gizmo_operation_change(int operation, ImGuiKey switch_key);
        void render_gizmos(const Set<Entity*>& entities);
	};
}

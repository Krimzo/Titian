#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "render/render_layer.h"
#include "serialization/serializer.h"


namespace titian {
	class GUISectionViewport : public GUISection
	{
	public:
        GUISectionViewport(const LayerPackage& package);

        void render_gui() override;

    private:
        Map<ImGuiKey, bool> m_last_key_states = {};
        Optional<Int2> m_rect_selection_first = {};

        Int2 window_mouse_position();
        Set<uint32_t> read_entity_ids(const Int2& first_coords, const Int2& second_coords);

        void handle_gizmo_operation_change(int operation, ImGuiKey switch_key);
        void render_gizmos(const Set<Entity*>& entities);
	};
}

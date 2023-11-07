#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "render/render_layer.h"
#include "serialization/serializer.h"


namespace titian {
	class GUISectionViewport : public GUISection
	{
	public:
        EditorLayer* editor_layer = nullptr;
        RenderLayer* render_layer = nullptr;

        GUISectionViewport(EditorLayer* editor_layer, RenderLayer* render_layer);

        void render_gui() override;

    private:
        std::map<ImGuiKey, bool> m_last_key_states = {};

        kl::Int2 window_mouse_position();
        uint32_t read_entity_id(const kl::Int2& pixel_coords);

        void handle_gizmo_operation_change(int operation, ImGuiKey switch_key);
        void render_gizmos(Entity* entity);
	};
}

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
        std::optional<kl::Int2> m_rect_selection_first = {};

        kl::Int2 window_mouse_position();
        std::unordered_set<uint32_t> read_entity_ids(const kl::Int2& first_coords, const kl::Int2& second_coords);

        void handle_gizmo_operation_change(int operation, ImGuiKey switch_key);
        void render_gizmos(const std::unordered_set<Entity*>& entities);
	};
}

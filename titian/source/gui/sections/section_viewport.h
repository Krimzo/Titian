#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "render/render_layer.h"
#include "serial/serial.h"


namespace titian {
    struct GUISectionViewport : GUISection
	{
        GUISectionViewport();

        void render_gui() override;

    private:
        Opt<Int2> m_rect_selection_first;
        Float3 m_last_scaling{ 1.0f };
        bool m_was_using = false;

        Int2 window_mouse_position() const;
        Float2 window_to_ndc(Int2 coords) const;

        Set<uint32_t> read_id_texture(Int2 first_coords, Int2 second_coords);
        Opt<Float3> read_depth_texture(Int2 coords);

        void render_gizmos(const Set<Entity*>& entities);
	};
}

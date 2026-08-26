#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "render/render_layer.h"
#include "serial/serial.h"

namespace titian
{
struct GUISectionViewport : GUISection
{
    GUISectionViewport();

    void render_gui() override;

  private:
    Opt<int2> m_rect_selection_first;
    float3 m_last_scaling{1.0f};
    bool m_was_using = false;

    int2 window_mouse_position() const;
    float2 window_to_ndc(int2 coords) const;

    Set<uint32_t> read_id_texture(int2 first_coords, int2 second_coords);
    Opt<float3> read_depth_texture(int2 coords);

    void render_gizmos(Set<Entity*> const& entities);
};
} // namespace titian

#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "render/render_layer.h"
#include "serial/serial.h"


namespace titian
{
struct GUISectionHelper : GUISection
{
    Texture basic_entity_texture;
    Texture animation_entity_texture;

    GUISectionHelper();

    void render_gui() override;

private:
    float m_icon_size = 70.0f;

    void handle_basic_entry( StringRef const& name ) const;
    void handle_animation_entry( StringRef const& name ) const;
};
}

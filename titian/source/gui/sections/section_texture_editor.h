#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    struct GUISectionTextureEditor : GUISection
    {
        String selected_texture = {};

        GUISectionTextureEditor();

        void render_gui() override;

    private:
        void display_textures(Scene* scene);
        void render_selected_texture(Texture* texture);
        void show_texture_properties(Texture* texture);
    };
}

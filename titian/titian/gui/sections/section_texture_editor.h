#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"


namespace titian {
    class GUISectionTextureEditor : public GUISection
    {
    public:
        String selected_texture = {};

        GUISectionTextureEditor(const LayerPackage& package);

        void render_gui() override;

    private:
        void display_textures(kl::GPU* gpu, Scene* scene);
        void render_selected_texture(Texture* texture, kl::Int2 viewport_size);
        void show_texture_properties(Texture* texture);
    };
}

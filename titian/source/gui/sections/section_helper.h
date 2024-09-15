#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "render/render_layer.h"
#include "serialization/serialization.h"


namespace titian {
    struct GUISectionHelper : GUISection
    {
        Ref<Texture> basic_entity_texture;
        Ref<Texture> animation_entity_texture;

        GUISectionHelper();

        void render_gui() override;

    private:
        float m_icon_size = 65.0f;

        void handle_basic_entry(const StringView& name) const;
        void handle_animation_entry(const StringView& name) const;
    };
}

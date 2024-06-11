#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"

#include "scripting/interp/language_editor.h"


namespace titian {
    class GUISectionShaderEditor : public GUISection
    {
    public:
        std::string selected_shader = {};

        GUISectionShaderEditor(const LayerPackage& package);

        void render_gui() override;

    private:
        LanguageEditor m_editor{};
        Shader* m_last_shader = nullptr;

        void display_shaders(Scene* scene);
        void edit_shader(Shader* shader);
        void show_shader_properties(Shader* shader) const;
    };
}

#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"

#include "scripting/interp/language_editor.h"


namespace titian {
    class GUISectionShaderEditor : public GUISection
    {
    public:
        EditorLayer* editor_layer = nullptr;
        GUILayer* gui_layer = nullptr;

        std::string selected_shader = {};

        GUISectionShaderEditor(EditorLayer* editor_layer, GUILayer* gui_layer);

        void render_gui() override;

    private:
        LanguageEditor m_editor;
        Shader* m_last_shader = nullptr;

        void display_shaders(Scene* scene);
        void edit_shader(Shader* shader);
        void show_shader_properties(Shader* shader) const;
    };
}

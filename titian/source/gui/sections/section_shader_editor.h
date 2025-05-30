#pragma once

#include "gui/gui_section.h"
#include "editor/editor_layer.h"
#include "gui/gui_layer.h"

#include "gui/editors/code_editor.h"


namespace titian
{
struct GUISectionShaderEditor : GUISection
{
    String selected_shader;

    GUISectionShaderEditor();

    void render_gui() override;

private:
    CodeEditor m_editor{};
    Shader* m_last_shader = nullptr;

    void display_shaders( Scene& scene );
    void edit_shader( Shader& shader );
    void show_shader_properties( Shader* shader ) const;
};
}

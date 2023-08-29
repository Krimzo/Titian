#include "editor.h"


void prepare_gui_frame(editor_state* state);
void finalize_gui_frame(editor_state* state);

void gui_render(editor_state* state)
{
    // Prepare
    prepare_gui_frame(state);

    // Top
    gui_main_menu(state);
    gui_control_menu(state);

    // Left
    gui_scene_items(state);
    gui_script_items(state);
    gui_scene_info(state);

    // Middle-Top
    gui_depth_display(state);
    gui_mesh_editor(state);
    gui_material_editor(state);
    gui_scene_editor(state);

    // Middle-Bottom
    gui_log_view(state);
    gui_explorer(state);

    // Right
    gui_entity_properties(state);
    gui_render_info(state);

    // Finalize
    finalize_gui_frame(state);
}

void prepare_gui_frame(editor_state* state)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
}

void finalize_gui_frame(editor_state* state)
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

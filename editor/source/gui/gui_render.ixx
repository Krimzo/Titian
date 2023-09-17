export module gui_render;

export import gui_helper;
export import render_chain;

// Top
export void gui_main_menu(editor_state* state);
export void gui_control_menu(editor_state* state);

// Left
export void gui_scene_items(editor_state* state);
export void gui_script_items(editor_state* state);
export void gui_scene_info(editor_state* state);

// Middle-Top
export void gui_mesh_editor(editor_state* state);
export void gui_material_editor(editor_state* state);
export void gui_scene_editor(editor_state* state);

// Middle-Bottom
export void gui_log_view(editor_state* state);
export void gui_explorer(editor_state* state);

// Right
export void gui_entity_properties(editor_state* state);
export void gui_render_info(editor_state* state);

export void gui_render(editor_state* state)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

    // Top
    gui_main_menu(state);
    gui_control_menu(state);

    // Left
    gui_scene_items(state);
    gui_script_items(state);
    gui_scene_info(state);

    // Middle-Top
    gui_mesh_editor(state);
    gui_material_editor(state);
    gui_scene_editor(state);

    // Middle-Bottom
    gui_log_view(state);
    gui_explorer(state);

    // Right
    gui_entity_properties(state);
    gui_render_info(state);

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

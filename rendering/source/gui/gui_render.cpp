#include "gui/gui_render.h"


void prepare_gui_frame(state_machine* state);
void finalize_gui_frame(state_machine* state);

void gui_render(state_machine* state)
{
    // Prepare
    prepare_gui_frame(state);

    // Left
    gui_scene_items(state);
    gui_scene_info(state);

    // Middle
    gui_depth_display(state);
    gui_viewport(state);

    // Right
    gui_entity_properties(state);

    // Finalize
    finalize_gui_frame(state);
}

void prepare_gui_frame(state_machine* state)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
}

void finalize_gui_frame(state_machine* state)
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

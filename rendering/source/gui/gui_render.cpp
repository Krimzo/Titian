#include "gui/gui_render.h"


void prepare_gui_frame(state_machine* state);
void finalize_gui_frame(state_machine* state);

void gui_render(state_machine* state)
{
    prepare_gui_frame(state);

    gui_scene_info(state);

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

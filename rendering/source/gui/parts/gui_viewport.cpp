#include "gui/gui_render.h"


kl::int2 get_window_mouse_position();
int get_entity_index(state_machine* state, const kl::int2& pixel_coords);

void gui_viewport(state_machine* state)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

    if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        // Pre-display
        const kl::int2 window_size = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };

        // Display scene buffer
        if (window_size.x > 0 && window_size.y > 0 && window_size != state->render_state->target_size) {
            state->render_state = kl::make<render_state>(state->gpu, window_size);
        }
        ImGui::Image(state->render_state->render_shader_view, ImGui::GetWindowSize());

        // Handle entity picking
        if (ImGui::IsWindowFocused() && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            const kl::int2 pixel_coords = get_window_mouse_position();
            const int entity_index = get_entity_index(state, pixel_coords);
            state->scene->update_selected_entity(entity_index);
        }
    }
    ImGui::End();

    ImGui::PopStyleVar();
}

kl::int2 get_window_mouse_position()
{
    const float tab_size = ImGui::GetWindowContentRegionMin().y;
    const ImVec2 window_position = ImGui::GetWindowPos();
    const ImVec2 mouse_position = ImGui::GetMousePos();
    return {
        (int) (mouse_position.x - window_position.x - 0.0f),
        (int) (mouse_position.y - window_position.y - tab_size),
    };
}

int get_entity_index(state_machine* state, const kl::int2& pixel_coords)
{
    if (pixel_coords.x < 0 || pixel_coords.y < 0) { return 0; }

    D3D11_BOX source_box = {};
    source_box.left = pixel_coords.x;
    source_box.right = source_box.left + 1;
    source_box.top = pixel_coords.y;
    source_box.bottom = source_box.top + 1;
    source_box.front = 0;
    source_box.back = 1;

    state->gpu->context()->CopySubresourceRegion(state->render_state->picking_staging_texture, 0, 0, 0, 0, state->render_state->picking_texture, 0, &source_box);

    float result = 0.0f;
    state->gpu->read_from_resource(&result, state->render_state->picking_staging_texture, sizeof(float));
    return (int) result;
}

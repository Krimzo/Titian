#include "gui/helper/gui_helper.h"


void GUI::set_drag_data(const char* id, const void* data, const size_t data_size, const kl::dx::shader_view& texture)
{
    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload(id, data, data_size);
        if (texture) ImGui::Image(texture.Get(), { 50.0f, 50.0f });
        ImGui::EndDragDropSource();
    }
}

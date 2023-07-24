#include "gui/helper/gui_helper.h"


void GUI::popup::on_window(const std::string& id, const std::function<void()>& callback)
{
    if (ImGui::BeginPopupContextWindow(id.c_str(), ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
        callback();
        ImGui::EndPopup();
    }
}

void GUI::popup::on_item(const std::string& id, const std::function<void()>&callback)
{
    if (ImGui::BeginPopupContextItem(id.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        callback();
        ImGui::EndPopup();
    }
}

void GUI::popup::close()
{
    ImGui::CloseCurrentPopup();
}

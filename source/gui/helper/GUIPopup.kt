package gui.helper

import imgui.ImGui
import imgui.flag.ImGuiPopupFlags

object GUIPopup {
    fun close() {
        ImGui.closeCurrentPopup()
    }

    fun windowPopup(name: String?, callback: () -> Unit) {
        if (ImGui.beginPopupContextWindow(name, ImGuiPopupFlags.MouseButtonRight or ImGuiPopupFlags.NoOpenOverItems)) {
            callback()
            ImGui.endPopup()
        }
    }

    fun itemPopup(name: String?, callback: () -> Unit) {
        if (ImGui.beginPopupContextItem(name, ImGuiPopupFlags.MouseButtonRight)) {
            callback()
            ImGui.endPopup()
        }
    }
}
package gui;

import callback.EmptyCallback;
import imgui.ImGui;
import imgui.flag.ImGuiPopupFlags;

public final class GUIPopup {
    private GUIPopup() {}

    public static void close() {
        ImGui.closeCurrentPopup();
    }

    public static void windowPopup(String name, EmptyCallback callback) {
        if (ImGui.beginPopupContextWindow(name, ImGuiPopupFlags.MouseButtonRight | ImGuiPopupFlags.NoOpenOverItems)) {
            callback.method();
            ImGui.endPopup();
        }
    }

    public static void itemPopup(String name, EmptyCallback callback) {
        if (ImGui.beginPopupContextItem(name, ImGuiPopupFlags.MouseButtonRight)) {
            callback.method();
            ImGui.endPopup();
        }
    }
}

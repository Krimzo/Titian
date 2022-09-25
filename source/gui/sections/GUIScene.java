package gui.sections;

import gui.GUIRenderable;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIScene implements GUIRenderable {
    public GUIScene() {}

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

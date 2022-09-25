package gui.sections;

import gui.GUIRenderable;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIExplorer implements GUIRenderable {
    public GUIExplorer() {}

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Explorer", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

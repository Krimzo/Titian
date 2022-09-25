package gui.sections;

import gui.GUIRenderable;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIProperties implements GUIRenderable {
    public GUIProperties() {}

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Properties", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

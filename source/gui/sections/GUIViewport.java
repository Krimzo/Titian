package gui.sections;

import gui.GUIRenderable;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIViewport implements GUIRenderable {
    public GUIViewport() {}

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Viewport", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

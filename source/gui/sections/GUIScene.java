package gui.sections;

import editor.Editor;
import gui.GUIRenderable;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIScene extends GUISection implements GUIRenderable {
    public GUIScene(Editor editor) {
        super(editor);
    }

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

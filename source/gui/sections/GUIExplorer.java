package gui.sections;

import editor.Editor;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIExplorer extends GUISection {
    public GUIExplorer(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Explorer", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

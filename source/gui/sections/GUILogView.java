package gui.sections;

import editor.Editor;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUILogView extends GUISection {
    public GUILogView(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Log View", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

package gui.sections;

import editor.Editor;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIControlPanel extends GUISection {
    public GUIControlPanel(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Control Panel", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

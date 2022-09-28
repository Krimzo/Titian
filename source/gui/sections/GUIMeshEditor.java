package gui.sections;

import editor.Editor;
import gui.GUISection;
import imgui.ImGui;

public final class GUIMeshEditor extends GUISection {
    public GUIMeshEditor(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Mesh Editor")) {

        }
        ImGui.end();
    }
}

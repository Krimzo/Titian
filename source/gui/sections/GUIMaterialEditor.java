package gui.sections;

import editor.Editor;
import gui.GUISection;
import imgui.ImGui;

public final class GUIMaterialEditor extends GUISection {
    public GUIMaterialEditor(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Material Editor")) {

        }
        ImGui.end();
    }
}
package gui.section;

import editor.Editor;
import gui.abs.GUISection;
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
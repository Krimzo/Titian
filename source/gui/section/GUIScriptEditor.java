package gui.section;

import editor.Editor;
import gui.abs.GUISection;
import imgui.ImGui;

public final class GUIScriptEditor extends GUISection {
    public GUIScriptEditor(Editor editor) {
        super(editor);
    }

    @Override
    public void free() {

    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Script Editor")) {

        }
        ImGui.end();
    }
}
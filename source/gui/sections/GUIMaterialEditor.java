package gui.sections;

import editor.Editor;
import gui.GUISection;
import imgui.ImGui;

public final class GUIMaterialEditor extends GUISection {
    public GUIMaterialEditor(Editor editor) {
        super(editor);
    }

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Material Editor")) {

        }
        ImGui.end();
    }
}
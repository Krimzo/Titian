package gui.sections;

import editor.Editor;
import gui.GUISection;
import imgui.ImGui;

public final class GUIMeshEditor extends GUISection {
    public GUIMeshEditor(Editor editor) {
        super(editor);
    }

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Mesh Editor")) {

        }
        ImGui.end();
    }
}

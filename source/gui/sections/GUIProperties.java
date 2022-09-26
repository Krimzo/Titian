package gui.sections;

import editor.Editor;
import gui.GUIRenderable;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIProperties extends GUISection implements GUIRenderable {
    public GUIProperties(Editor editor) {
        super(editor);
    }

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Properties", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

package gui.sections;

import editor.Editor;
import gui.GUIRenderable;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUILogView extends GUISection implements GUIRenderable {
    public GUILogView(Editor editor) {
        super(editor);
    }

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Log View", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

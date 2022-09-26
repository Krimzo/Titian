package gui.sections;

import editor.Editor;
import gui.GUIRenderable;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIGamePanel extends GUISection implements GUIRenderable {
    public GUIGamePanel(Editor editor) {
        super(editor);
    }

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Game Panel", ImGuiWindowFlags.NoScrollbar)) {

        }
        ImGui.end();
    }
}

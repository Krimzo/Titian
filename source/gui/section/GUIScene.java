package gui.section;

import editor.Editor;
import entity.Entity;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIScene extends GUISection {
    public GUIScene(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar)) {
            if (editor.scene != null) {
                for (Entity entity : editor.scene) {
                    if (ImGui.selectable(entity.getName(), entity == editor.scene.selectedEntity)) {
                        editor.scene.selectedEntity = entity;
                    }
                }
            }
        }
        ImGui.end();
    }
}

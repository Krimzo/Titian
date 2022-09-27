package gui.sections;

import editor.Editor;
import entity.Entity;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;
import scene.Scene;

public final class GUIScene extends GUISection {
    public GUIScene(Editor editor) {
        super(editor);
    }

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar)) {
            Scene scene = editor.getScene();
            if (scene != null) {
                for (Entity entity : scene) {
                    if (ImGui.selectable(entity.getName(), entity == scene.selectedEntity)) {
                        scene.selectedEntity = entity;
                    }
                }
            }
        }
        ImGui.end();
    }
}

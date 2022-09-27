package gui.sections;

import editor.*;
import entity.Entity;
import gui.*;
import imgui.*;
import imgui.flag.*;
import scene.*;

public final class GUIProperties extends GUISection {
    public GUIProperties(Editor editor) {
        super(editor);
    }

    private Entity getSelectedEntity() {
        Scene scene = editor.getScene();
        if (scene != null) {
            return scene.selectedEntity;
        }
        return null;
    }

    @Override
    public void onGUIRender() {
        Entity selected = getSelectedEntity();

        if (ImGui.begin("Transform", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) selected.transformComponent.onGUIRender();
        }
        ImGui.end();

        if (ImGui.begin("Mesh", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) selected.meshComponent.onGUIRender();
        }
        ImGui.end();

        if (ImGui.begin("Material", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) selected.materialComponent.onGUIRender();
        }
        ImGui.end();

        if (ImGui.begin("Physics", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) selected.physicsComponent.onGUIRender();
        }
        ImGui.end();
    }
}

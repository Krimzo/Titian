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
    public void renderGUI() {
        Entity selected = getSelectedEntity();

        if (ImGui.begin("Transform", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) selected.transformComponent.renderGUI();
        }
        ImGui.end();

        if (ImGui.begin("Mesh", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) selected.meshComponent.renderGUI();
        }
        ImGui.end();

        if (ImGui.begin("Material", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) selected.materialComponent.renderGUI();
        }
        ImGui.end();

        if (ImGui.begin("Physics", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) selected.physicsComponent.renderGUI();
        }
        ImGui.end();
    }
}

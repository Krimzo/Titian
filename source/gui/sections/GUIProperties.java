package gui.sections;

import editor.*;
import entity.Entity;
import glparts.Mesh;
import gui.*;
import imgui.*;
import imgui.flag.*;
import material.Material;

public final class GUIProperties extends GUISection {
    public GUIProperties(Editor editor) {
        super(editor);
    }

    private Entity getSelectedEntity() {
        if (editor.scene != null) {
            return editor.scene.selectedEntity;
        }
        return null;
    }

    @Override
    public void renderGUI() {
        Entity selected = getSelectedEntity();

        if (ImGui.begin("Transform", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) {
                selected.transformComponent.renderGUI();
            }
        }
        ImGui.end();

        if (ImGui.begin("Mesh", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) {
                selected.meshComponent.renderGUI();
                ImGui.separator();

                for (Mesh mesh : editor.scene.meshes) {
                    if (ImGui.selectable(mesh.getName())) {
                        selected.meshComponent.mesh = mesh;
                    }
                }
            }
        }
        ImGui.end();

        if (ImGui.begin("Material", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) {
                selected.materialComponent.renderGUI();
                ImGui.separator();

                for (Material material : editor.scene.materials) {
                    if (ImGui.selectable(material.getName())) {
                        selected.materialComponent.material = material;
                    }
                }
            }
        }
        ImGui.end();

        if (ImGui.begin("Physics", ImGuiWindowFlags.NoScrollbar)) {
            if (selected != null) {
                selected.physicsComponent.renderGUI();
            }
        }
        ImGui.end();
    }
}

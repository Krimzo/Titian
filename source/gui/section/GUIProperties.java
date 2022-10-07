package gui.section;

import editor.*;
import entity.Entity;
import glparts.Mesh;
import gui.abs.GUISection;
import imgui.*;
import imgui.flag.*;
import material.Material;
import script.Script;

public final class GUIProperties extends GUISection {
    public GUIProperties(Editor editor) {
        super(editor);
    }

    private void renderTransform(Entity selected) {
        selected.transformComponent.renderGUI();
    }

    private void renderMesh(Entity selected) {
        selected.meshComponent.renderGUI();

        if (ImGui.beginListBox("##Loaded meshes", -1, 0)) {
            for (Mesh mesh : editor.scene.meshes) {
                if (ImGui.selectable(mesh.getName(), selected.meshComponent.mesh == mesh)) {
                    selected.meshComponent.mesh = mesh;
                }
            }

            ImGui.endListBox();
        }
    }

    private void renderMaterial(Entity selected) {
        selected.materialComponent.renderGUI();

        if (ImGui.beginListBox("##Loaded materials", -1, 0)) {
            for (Material material : editor.scene.materials) {
                if (ImGui.selectable(material.getName(), selected.materialComponent.material == material)) {
                    selected.materialComponent.material = material;
                }
            }

            ImGui.endListBox();
        }
    }

    private void renderPhysics(Entity selected) {
        selected.physicsComponent.renderGUI();
    }

    private void renderScripts(Entity selected) {
        selected.scriptComponent.renderGUI();

        ImVec2 contentMin = ImGui.getWindowContentRegionMin();
        ImVec2 contentMax = ImGui.getWindowContentRegionMax();
        ImGui.dummy(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

        if (ImGui.beginDragDropTarget()) {
            String scriptPath = ImGui.acceptDragDropPayload("ScriptFile");
            if (scriptPath != null) {
                selected.scriptComponent.scripts.add(new Script(scriptPath, selected));
            }

            ImGui.endDragDropTarget();
        }

        if (ImGui.beginPopupContextItem("Scripts", ImGuiPopupFlags.MouseButtonRight | ImGuiPopupFlags.NoOpenOverItems)) {
            if (ImGui.button("Reload")) {
                selected.scriptComponent.reload();

                ImGui.closeCurrentPopup();
            }

            ImGui.endPopup();
        }
    }

    @Override
    public void renderGUI() {
        Entity selected = (editor.scene != null) ? editor.scene.selectedEntity : null;

        if (ImGui.begin("Properties") && selected != null) {
            if (ImGui.collapsingHeader("Info", ImGuiTreeNodeFlags.DefaultOpen)) {
                selected.renderInfoGUI(editor);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Transform", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderTransform(selected);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Mesh", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderMesh(selected);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Material", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderMaterial(selected);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Physics", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderPhysics(selected);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Scripts", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderScripts(selected);
            }
        }
        ImGui.end();
    }
}

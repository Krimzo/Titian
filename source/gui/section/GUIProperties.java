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

    private boolean renderTransform(Entity selected) {
        if (!ImGui.collapsingHeader("Transform", ImGuiTreeNodeFlags.DefaultOpen)) {
            return false;
        }

        selected.transformComponent.renderGUI();

        return true;
    }

    private boolean renderMesh(Entity selected) {
        if (!ImGui.collapsingHeader("Mesh", ImGuiTreeNodeFlags.DefaultOpen)) {
            return false;
        }

        selected.meshComponent.renderGUI();

        if (ImGui.beginListBox("##Loaded meshes", -1, 0)) {
            for (Mesh mesh : editor.scene.meshes) {
                if (ImGui.selectable(mesh.getName(), selected.meshComponent.mesh == mesh)) {
                    selected.meshComponent.mesh = mesh;
                }
            }

            ImGui.endListBox();
        }

        return true;
    }

    private boolean renderMaterial(Entity selected) {
        if (!ImGui.collapsingHeader("Material", ImGuiTreeNodeFlags.DefaultOpen)) {
            return false;
        }

        selected.materialComponent.renderGUI();

        if (ImGui.beginListBox("##Loaded materials", -1, 0)) {
            for (Material material : editor.scene.materials) {
                if (ImGui.selectable(material.getName(), selected.materialComponent.material == material)) {
                    selected.materialComponent.material = material;
                }
            }

            ImGui.endListBox();
        }

        return true;
    }

    private boolean renderPhysics(Entity selected) {
        if (!ImGui.collapsingHeader("Physics", ImGuiTreeNodeFlags.DefaultOpen)) {
            return false;
        }

        selected.physicsComponent.renderGUI();

        return true;
    }

    private boolean renderScripts(Entity selected) {
        if (!ImGui.collapsingHeader("Scripts", ImGuiTreeNodeFlags.DefaultOpen)) {
            return false;
        }

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

        return true;
    }

    @Override
    public void renderGUI() {
        Entity selected = (editor.scene != null) ? editor.scene.selectedEntity : null;

        if (ImGui.begin("Properties") && selected != null) {
            if (selected.renderInfoGUI(editor)) {
                ImGui.separator();
            }

            if (renderTransform(selected)) {
                ImGui.separator();
            }

            if (renderMesh(selected)) {
                ImGui.separator();
            }

            if (renderMaterial(selected)) {
                ImGui.separator();
            }

            if (renderPhysics(selected)) {
                ImGui.separator();
            }

            renderScripts(selected);
        }
        ImGui.end();
    }
}
